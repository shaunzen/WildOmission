// Copyright Telephone Studios. All Rights Reserved.


#include "PlayerAimComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/EquipComponent.h"
#include "Items/FirearmItem.h"
#include "WildOmissionGameUserSettings.h"

// Sets default values for this component's properties
UPlayerAimComponent::UPlayerAimComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	LookSensitivity = 1.0f;
	Aiming = false;
}

// Called when the game starts
void UPlayerAimComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPlayerAimComponent::SetCameraFOV(float NewFOV)
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
	{
		return;
	}

	UCameraComponent* CameraComponent = OwnerActor->FindComponentByClass<UCameraComponent>();
	if (CameraComponent == nullptr)
	{
		return;
	}

	CameraComponent->FieldOfView = NewFOV;
}

void UPlayerAimComponent::GetCurrentArmOffset(FVector& OutCurrentLocationOffset, FRotator& OutCurrentRotationOffset) const
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
	{
		return;
	}

	UEquipComponent* EquipComponent = OwnerActor->FindComponentByClass<UEquipComponent>();
	if (EquipComponent == nullptr)
	{
		return;
	}

	OutCurrentLocationOffset = EquipComponent->GetCurrentArmLocationOffset();
	OutCurrentRotationOffset = EquipComponent->GetCurrentArmRotationOffset();
}

void UPlayerAimComponent::SetArmOffset(const FVector& InNewLocationOffset, const FRotator& InNewRotationOffset)
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
	{
		return;
	}

	UEquipComponent* EquipComponent = OwnerActor->FindComponentByClass<UEquipComponent>();
	if (EquipComponent == nullptr)
	{
		return;
	}

	EquipComponent->SetArmLocationOffset(InNewLocationOffset);
	EquipComponent->SetArmRotationOffset(InNewRotationOffset);
}

float UPlayerAimComponent::GetCurrentCameraFOV() const
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
	{
		return 0.0f;
	}

	UCameraComponent* CameraComponent = OwnerActor->FindComponentByClass<UCameraComponent>();
	if (CameraComponent == nullptr)
	{
		return 0.0f;
	}

	return CameraComponent->FieldOfView;
}

FVector UPlayerAimComponent::GetTargetArmLocationOffset() const
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
	{
		return FVector::ZeroVector;
	}

	UEquipComponent* EquipComponent = OwnerActor->FindComponentByClass<UEquipComponent>();
	if (EquipComponent == nullptr)
	{
		return FVector::ZeroVector;
	}

	AFirearmItem* EquipedFirearm = Cast<AFirearmItem>(EquipComponent->GetEquipedItem());
	if (EquipedFirearm == nullptr)
	{
		return FVector::ZeroVector;
	}

	return EquipedFirearm->GetAimArmLocationOffset();
}

FRotator UPlayerAimComponent::GetTargetArmRotationOffset() const
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
	{
		return FRotator::ZeroRotator;
	}

	UEquipComponent* EquipComponent = OwnerActor->FindComponentByClass<UEquipComponent>();
	if (EquipComponent == nullptr)
	{
		return FRotator::ZeroRotator;
	}

	AFirearmItem* EquipedFirearm = Cast<AFirearmItem>(EquipComponent->GetEquipedItem());
	if (EquipedFirearm == nullptr)
	{
		return FRotator::ZeroRotator;
	}

	return EquipedFirearm->GetAimArmRotationOffset();
}

// Called every frame
void UPlayerAimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UWorld* World = GetWorld();
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (World == nullptr
		|| OwnerPawn == nullptr
		|| !OwnerPawn->IsLocallyControlled()
		|| !OwnerPawn->IsPlayerControlled()
		|| UserSettings == nullptr)
	{
		return;
	}

	// Calculate Sensitivity
	const float SettingsSensitivity = UserSettings->GetMouseSensitivity();
	const float SensitivityAimMultiplier = Aiming ? 0.25f : 1.0f;
	LookSensitivity = SettingsSensitivity * SensitivityAimMultiplier;

	// Calculate FOV
	const float SettingsFOV = UserSettings->GetFieldOfView();
	const float MaxFOVZoom = 10.0f;
	float CurrentFOV = GetCurrentCameraFOV();

	// Calculate Arm Offset
	const FVector TargetAimArmLocationOffset = GetTargetArmLocationOffset();
	const FRotator TargetAimArmRotationOffset = GetTargetArmRotationOffset();

	const FVector DefaultArmLocation = FVector(-5.0f, 0.0f, -160.0f);

	FVector CurrentArmLocationOffset = FVector::ZeroVector;
	FRotator CurrentArmRotationOffset = FRotator::ZeroRotator;

	GetCurrentArmOffset(CurrentArmLocationOffset, CurrentArmRotationOffset);

	const float InterpSpeed = 50.0f;

	if (Aiming)
	{
		// Update fov
		CurrentFOV = FMath::Clamp(CurrentFOV - InterpSpeed * World->GetDeltaSeconds(), SettingsFOV - MaxFOVZoom, SettingsFOV);

		// Move the arms
		CurrentArmLocationOffset = FMath::Lerp(CurrentArmLocationOffset, TargetAimArmLocationOffset, FMath::Clamp(InterpSpeed * World->GetDeltaSeconds(), 0.0f, 1.0f));
		CurrentArmRotationOffset = FMath::Lerp(CurrentArmRotationOffset, TargetAimArmRotationOffset, FMath::Clamp(InterpSpeed * World->GetDeltaSeconds(), 0.0f, 1.0f));
	}
	else
	{
		// Update fov
		CurrentFOV = FMath::Clamp(CurrentFOV + InterpSpeed * World->GetDeltaSeconds(), SettingsFOV - MaxFOVZoom, SettingsFOV);

		// Move the arms
		CurrentArmLocationOffset = FMath::Lerp(CurrentArmLocationOffset, FVector::ZeroVector, FMath::Clamp(InterpSpeed * World->GetDeltaSeconds(), 0.0f, 1.0f));
		CurrentArmRotationOffset = FMath::Lerp(CurrentArmRotationOffset, FRotator::ZeroRotator, FMath::Clamp(InterpSpeed * World->GetDeltaSeconds(), 0.0f, 1.0f));
	}

	SetCameraFOV(CurrentFOV);

	SetArmOffset(CurrentArmLocationOffset, CurrentArmRotationOffset);
}

float UPlayerAimComponent::GetLookSensitivity() const
{
	return LookSensitivity;
}

bool UPlayerAimComponent::IsAiming() const
{
	return Aiming;
}

void UPlayerAimComponent::StartAiming()
{
	Aiming = true;
}

void UPlayerAimComponent::StopAiming()
{
	Aiming = false;
}

