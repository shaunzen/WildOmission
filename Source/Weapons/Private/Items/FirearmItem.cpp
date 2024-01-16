// Copyright Telephone Studios. All Rights Reserved.


#include "Items/FirearmItem.h"
#include "Components/EquipComponent.h"
#include "Components/InventoryComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "WildOmissionGameUserSettings.h"
#include "UObject/ConstructorHelpers.h"

AFirearmItem::AFirearmItem()
{
	FireMontage = nullptr;
	FireItemMontage = nullptr;

	ReloadMontage = nullptr;
	ReloadItemMontage = nullptr;

	MuzzleFlashEffect = nullptr;
	
	AimArmLocationOffset = FVector::ZeroVector;
	AimArmRotationOffset = FRotator::ZeroRotator;

	RateOfFireRPM = 600;
	CanFire = true;

	MuzzleComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleComponent"));
	MuzzleComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> MuzzleFlashEffectBlueprint(TEXT("/Game/Weapons/Art/NS_MuzzleFlash"));
	if (MuzzleFlashEffectBlueprint.Succeeded())
	{
		MuzzleFlashEffect = MuzzleFlashEffectBlueprint.Object;
	}

	static ConstructorHelpers::FClassFinder<UCameraShakeBase> FireCameraShakeBlueprint(TEXT("/Game/Weapons/Effects/CS_LightImpulse"));
	if (FireCameraShakeBlueprint.Succeeded())
	{
		FireCameraShake = FireCameraShakeBlueprint.Class;
	}

	static ConstructorHelpers::FClassFinder<UCameraShakeBase> ReloadStartCameraShakeBlueprint(TEXT("/Game/Weapons/Effects/CS_ReloadStart"));
	if (ReloadStartCameraShakeBlueprint.Succeeded())
	{
		ReloadStartCameraShake = ReloadStartCameraShakeBlueprint.Class;
	}

	static ConstructorHelpers::FClassFinder<UCameraShakeBase> ReloadClimaxCameraShakeBlueprint(TEXT("/Game/Weapons/Effects/CS_ReloadClimax"));
	if (ReloadClimaxCameraShakeBlueprint.Succeeded())
	{
		ReloadClimaxCameraShake = ReloadClimaxCameraShakeBlueprint.Class;
	}
}

void AFirearmItem::Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID)
{
	Super::Equip(InOwnerPawn, InThirdPersonMeshComponent, InItemName, InFromSlotIndex, InUniqueID);

	FItemData* ItemData = UInventoryComponent::GetItemData(InItemName);
	if (ItemData == nullptr)
	{
		return;
	}

	float RPMData = ItemData->GetStat(TEXT("RateOfFireRPM"));
	if (RPMData == -1)
	{
		return;
	}

	RateOfFireRPM = RPMData;
}

void AFirearmItem::OnPrimaryPressed()
{
	Super::OnPrimaryPressed();

}

void AFirearmItem::OnPrimaryAnimationClimax(bool FromFirstPersonInstance)
{
	Super::OnPrimaryAnimationClimax(FromFirstPersonInstance);

	Fire();
}

void AFirearmItem::OnSecondaryPressed()
{
	Super::OnSecondaryPressed();

	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	OwnerEquipComponent->StartAim();
}

void AFirearmItem::OnSecondaryReleased()
{
	Super::OnSecondaryReleased();

	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	OwnerEquipComponent->StopAim();
}

void AFirearmItem::OnReloadPressed()
{
	Super::OnReloadPressed();

	if (GetOwner() == nullptr 
		|| GetOwnerPawn() == nullptr 
		|| GetOwnerEquipComponent() == nullptr 
		|| CurrentAmmo == MaxAmmo
		|| GetRemainingAmmoInInventory() == 0
		|| GetOwnerEquipComponent()->IsMontagePlaying(ReloadMontage))
	{
		return;
	}

	GetOwnerEquipComponent()->PlayItemMontage(ReloadMontage, ReloadItemMontage);
	PlayReloadStartCameraShake();
}

void AFirearmItem::OnReloadAnimationClimax(bool FromFirstPersonInstance)
{
	Super::OnReloadAnimationClimax(FromFirstPersonInstance);

	Reload();
	PlayReloadClimaxCameraShake();
}

FVector AFirearmItem::GetAimArmLocationOffset() const
{
	return AimArmLocationOffset;
}

FRotator AFirearmItem::GetAimArmRotationOffset() const
{
	return AimArmRotationOffset;
}

int32 AFirearmItem::GetRateOfFire() const
{
	return RateOfFireRPM;
}

void AFirearmItem::PlayFireEffects()
{
	Super::PlayFireEffects();
	PlayMuzzleFlash();
	PlayFireCameraShake();
}

void AFirearmItem::PlayMuzzleFlash()
{
	UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerEquipComponent == nullptr || MuzzleFlashEffect == nullptr)
	{
		return;
	}

	const FRotator FirstPersonItemWorldRotation = OwnerEquipComponent->GetFirstPersonItemComponent()->GetComponentRotation();
	const FVector MuzzleLocation = GetOwnerPawn()->IsLocallyControlled()
		? OwnerEquipComponent->GetFirstPersonItemComponent()->GetComponentLocation() + FirstPersonItemWorldRotation.RotateVector(MuzzleComponent->GetRelativeLocation())
		: MuzzleComponent->GetComponentLocation();
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleFlashEffect, MuzzleLocation, GetOwnerEquipComponent()->GetOwnerControlRotation());
}

void AFirearmItem::InvokeFireCooldownTimer()
{
	CanFire = false;
	FTimerHandle ResetCanFireTimerHandle;
	FTimerDelegate ResetCanFireTimerDelegate;
	ResetCanFireTimerDelegate.BindUObject(this, &AFirearmItem::ResetCanFire);

	const float CooldownTime = 60.0f / (float)RateOfFireRPM;
	GetWorld()->GetTimerManager().SetTimer(ResetCanFireTimerHandle, ResetCanFireTimerDelegate, CooldownTime, false);
}

void AFirearmItem::PlayFireCameraShake()
{
	if (GetOwnerPawn() == nullptr || FireCameraShake == nullptr || !GetOwnerPawn()->IsLocallyControlled())
	{
		return;
	}

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr || UserSettings->GetCameraShakeEnabled() == false)
	{
		return;
	}

	APlayerController* OwnerPlayerController = Cast<APlayerController>(GetOwnerPawn()->GetController());
	if (OwnerPlayerController == nullptr)
	{
		return;
	}

	OwnerPlayerController->ClientStartCameraShake(FireCameraShake);
}

void AFirearmItem::PlayReloadStartCameraShake()
{
	if (GetOwnerPawn() == nullptr || ReloadStartCameraShake == nullptr || !GetOwnerPawn()->IsLocallyControlled())
	{
		return;
	}

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr || UserSettings->GetCameraShakeEnabled() == false)
	{
		return;
	}

	APlayerController* OwnerPlayerController = Cast<APlayerController>(GetOwnerPawn()->GetController());
	if (OwnerPlayerController == nullptr)
	{
		return;
	}

	OwnerPlayerController->ClientStartCameraShake(ReloadStartCameraShake);
}

void AFirearmItem::PlayReloadClimaxCameraShake()
{
	if (GetOwnerPawn() == nullptr || ReloadClimaxCameraShake == nullptr || !GetOwnerPawn()->IsLocallyControlled())
	{
		return;
	}

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr || UserSettings->GetCameraShakeEnabled() == false)
	{
		return;
	}

	APlayerController* OwnerPlayerController = Cast<APlayerController>(GetOwnerPawn()->GetController());
	if (OwnerPlayerController == nullptr)
	{
		return;
	}

	OwnerPlayerController->ClientStartCameraShake(ReloadClimaxCameraShake);
}

void AFirearmItem::ResetCanFire()
{
	CanFire = true;
}

void AFirearmItem::Fire()
{
	PlayFireEffects();
	DecreaseAmmoAndDurability();
	InvokeFireCooldownTimer();
}