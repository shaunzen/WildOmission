// Copyright Telephone Studios. All Rights Reserved.


#include "Items/EquipableItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InventoryComponent.h"
#include "Components/InventoryManipulatorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/EquipComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AEquipableItem::AEquipableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// setup item mesh, for some reason we cant modify it from the editor
	// game seems to be crashing because no mesh is specified and we are trying to use the mesh to create a first person decoy
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetAnimInstanceClass(UAnimInstance::StaticClass());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SocketOffset = FTransform::Identity;

	RightArmVelocityOffset = FRotator::ZeroRotator;
	LeftArmVelocityOffset = FRotator::ZeroRotator;

	UseLeftHandMount = false;

	RootComponent = MeshComponent;

	EquipMontage = nullptr;

	// TODO move these to child classes
	EquipItemMontage = nullptr;
	EquipPose = nullptr;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EquipMontageAsset(TEXT("/Game/WildOmissionCore/Characters/Human/Animation/Items/A_Human_Tool_Equip_01_Montage"));
	if (EquipMontageAsset.Succeeded())
	{
		EquipMontage = EquipMontageAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> EquipPoseAsset(TEXT("/Game/WildOmissionCore/Characters/Human/Animation/Items/A_Human_Tool_Hold_01"));
	if (EquipPoseAsset.Succeeded())
	{
		EquipPose = EquipPoseAsset.Object;
	}

	FromSlotIndex = -1;
	UniqueID = 0;

	bPrimaryEnabled = true;
	bSecondaryEnabled = true;
}

// Called when the game starts or when spawned
void AEquipableItem::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent->SetRelativeLocation(SocketOffset.GetLocation());
	MeshComponent->SetRelativeRotation(SocketOffset.GetRotation());
}

APawn* AEquipableItem::GetOwnerPawn() const
{
	return Cast<APawn>(GetOwner());
}

UEquipComponent* AEquipableItem::GetOwnerEquipComponent() const
{
	if (GetOwner() == nullptr)
	{
		return nullptr;
	}

	return GetOwner()->FindComponentByClass<UEquipComponent>();
}

FInventoryItem* AEquipableItem::FindInInventory() const
{
	UInventoryComponent* OwnerInventory = Owner->FindComponentByClass<UInventoryComponent>();
	UInventoryManipulatorComponent* OwnerInventoryManipulatorComponent = Owner->FindComponentByClass<UInventoryManipulatorComponent>();
	if (OwnerInventory == nullptr || OwnerInventoryManipulatorComponent == nullptr)
	{
		return nullptr;
	}

	FInventoryItem* InventoryItem = OwnerInventory->FindItemWithUniqueID(UniqueID);
	if (InventoryItem == nullptr)
	{
		if (!OwnerInventoryManipulatorComponent->SelectedItemHasUniqueID(UniqueID))
		{
			return nullptr;
		}

		InventoryItem = OwnerInventoryManipulatorComponent->GetSelectedItemAddress();
	}

	return InventoryItem;
}

// Called every frame
void AEquipableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AEquipableItem::Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID)
{
	SetOwner(InOwnerPawn);
	
	ItemName = InItemName;
	FromSlotIndex = InFromSlotIndex;
	UniqueID = InUniqueID;

	UseLeftHandMount ?
	AttachToComponent(InThirdPersonMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("LeftHandMountSocket")) :
	AttachToComponent(InThirdPersonMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("RightHandMountSocket"));

	GetOwnerEquipComponent()->PlayItemMontage(EquipMontage, EquipItemMontage);
}

void AEquipableItem::OnUnequip()
{

}

void AEquipableItem::OnPrimaryPressed()
{

}

void AEquipableItem::OnPrimaryHeld()
{

}

void AEquipableItem::OnPrimaryReleased()
{

}

void AEquipableItem::OnPrimaryAnimationClimax(bool FromFirstPersonInstance)
{

}

void AEquipableItem::OnSecondaryPressed()
{

}

void AEquipableItem::OnSecondaryHeld()
{

}

void AEquipableItem::OnSecondaryReleased()
{

}

void AEquipableItem::OnSecondaryAnimationClimax(bool FromFirstPersonInstance)
{

}

void AEquipableItem::OnReloadPressed()
{

}

void AEquipableItem::OnReloadAnimationClimax(bool FromFirstPersonInstance)
{

}

USkeletalMeshComponent* AEquipableItem::GetMeshComponent() const
{
	return MeshComponent;
}

void AEquipableItem::SetLocalVisibility(bool bVisible)
{
	MeshComponent->SetVisibility(bVisible);
}

int8 AEquipableItem::GetFromSlotIndex() const
{
	return FromSlotIndex;
}

void AEquipableItem::SetUniqueItemID(const uint32& InUniqueID)
{
	UniqueID = InUniqueID;
}

uint32 AEquipableItem::GetUniqueItemID() const
{
	return UniqueID;
}

UAnimMontage* AEquipableItem::GetEquipMontage() const
{
	return EquipMontage;
}

UAnimMontage* AEquipableItem::GetEquipItemMontage() const
{
    return EquipItemMontage;
}

UAnimSequence* AEquipableItem::GetEquipPose() const
{
	return EquipPose;
}

FTransform AEquipableItem::GetSocketOffset() const
{
	return SocketOffset;
}

FRotator AEquipableItem::GetRightArmVelocityOffset() const
{
	return RightArmVelocityOffset;
}

FRotator AEquipableItem::GetLeftArmVelocityOffset() const
{
	return LeftArmVelocityOffset;
}

bool AEquipableItem::IsOwnerOurPlayer() const
{
	APawn* OwnerPawn = GetOwnerPawn();
	if (OwnerPawn == nullptr)
	{
		return false;
	}

	AController* OwnerPawnController = OwnerPawn->GetController();
	if (OwnerPawnController == nullptr)
	{
		return false;
	}

	return OwnerPawn->IsLocallyControlled() && OwnerPawnController->IsPlayerController();
}

bool AEquipableItem::IsLeftHandMounted() const
{
	return UseLeftHandMount;
}

bool AEquipableItem::PrimaryEnabled() const
{
	return bPrimaryEnabled;
}

bool AEquipableItem::SecondaryEnabled() const
{
	return bSecondaryEnabled;
}