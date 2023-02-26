// Copyright Telephone Studios. All Rights Reserved.


#include "EquipableItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"

// Sets default values
AEquipableItem::AEquipableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	FirstPersonMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("FirstPersonMesh"));
	FirstPersonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FirstPersonMesh->SetCastShadow(false);
	FirstPersonMesh->SetupAttachment(RootComponent);

	ThirdPersonMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ThirdPersonMesh"));
	ThirdPersonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ThirdPersonMesh->SetCastShadow(true);
	ThirdPersonMesh->SetupAttachment(RootComponent);
	
	ConstructorHelpers::FObjectFinder<USoundBase> EquipSoundObject(TEXT("/Game/WildOmission/Items/EquipableItems/Audio/EquipDefault/Equip_Default_Cue"));

	if (EquipSoundObject.Object == nullptr)
	{
		return;
	}

	EquipSound = EquipSoundObject.Object;
}

// Called when the game starts or when spawned
void AEquipableItem::BeginPlay()
{
	Super::BeginPlay();

	if (IsOwnedByOurLocalPlayer())
	{
		FirstPersonMesh->SetVisibility(true);
		ThirdPersonMesh->SetVisibility(false);
	}
	else
	{
		FirstPersonMesh->SetVisibility(false);
		ThirdPersonMesh->SetVisibility(true);
	}

	FirstPersonMesh->SetStaticMesh(ItemMesh);
	ThirdPersonMesh->SetStaticMesh(ItemMesh);
}

// Called every frame
void AEquipableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AEquipableItem::Equip(AWildOmissionCharacter* InOwnerCharacter)
{
	SetOwner(InOwnerCharacter);
	HandleAttachment();
	Client_PlayEquipSound();
}

void AEquipableItem::OnUnequip()
{
	Client_PlayEquipSound();
}

void AEquipableItem::Primary()
{

}

void AEquipableItem::Secondary()
{

}

AWildOmissionCharacter* AEquipableItem::GetOwnerCharacter() const
{
	return Cast<AWildOmissionCharacter>(GetOwner());
}

bool AEquipableItem::IsOwnedByOurLocalPlayer() const
{
	AWildOmissionCharacter* OwnerCharacter = GetOwnerCharacter();
	if (OwnerCharacter == nullptr)
	{

		return false;
	}

	return OwnerCharacter->IsLocallyControlled();
}

void AEquipableItem::HandleAttachment()
{

	AWildOmissionCharacter* OwnerCharacter = GetOwnerCharacter();

	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner was nullptr"));
		return;
	}
	
	FirstPersonMesh->AttachToComponent(OwnerCharacter->GetFirstPersonMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightHandMountSocket"));
	ThirdPersonMesh->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightHandMountSocket"));
}

void AEquipableItem::Client_PlayEquipSound_Implementation()
{
	if (GetWorld() == nullptr || EquipSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), EquipSound, GetActorLocation());
}