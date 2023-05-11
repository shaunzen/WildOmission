// Copyright Telephone Studios. All Rights Reserved.


#include "EquipableItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Components/EquipComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AEquipableItem::AEquipableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	// setup item mesh, for some reason we cant modify it from the editor
	// game seems to be crashing because no mesh is specified and we are trying to use the mesh to create a first person decoy
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RootComponent = Mesh;

	ConstructorHelpers::FObjectFinder<UAnimMontage> EquipMontageAsset(TEXT("/Game/WildOmission/Characters/Human/Animation/Items/A_Human_EquipTool_01_Montage"));
	ConstructorHelpers::FObjectFinder<UAnimSequence> EquipPoseAsset(TEXT("/Game/WildOmission/Characters/Human/Animation/Items/A_Human_HoldTool_01"));

	if (EquipMontageAsset.Succeeded())
	{
		EquipMontage = EquipMontageAsset.Object;
	}

	if (EquipPoseAsset.Succeeded())
	{
		EquipPose = EquipPoseAsset.Object;
	}

	FromSlotIndex = -1;
	UniqueID = 0;

	bPrimaryEnabled = true;
	bSecondaryEnabled = true;
}

void AEquipableItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEquipableItem, ItemName);
	DOREPLIFETIME(AEquipableItem, FromSlotIndex);
	DOREPLIFETIME(AEquipableItem, UniqueID);
}

// Called when the game starts or when spawned
void AEquipableItem::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetRelativeLocation(SocketOffset.GetLocation());
	Mesh->SetRelativeRotation(SocketOffset.GetRotation());
}

// Called every frame
void AEquipableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AEquipableItem::Equip(AWildOmissionCharacter* InOwnerCharacter, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID)
{
	SetOwner(InOwnerCharacter);
	
	ItemName = InItemName;
	FromSlotIndex = InFromSlotIndex;
	UniqueID = InUniqueID;

	AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("RightHandMountSocket"));

	Client_PlayThirdPersonEquipMontage();
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

void AEquipableItem::OnSecondaryPressed()
{

}

void AEquipableItem::OnSecondaryHeld()
{

}

void AEquipableItem::OnSecondaryReleased()
{

}

UStaticMesh* AEquipableItem::GetMesh()
{
	return Mesh->GetStaticMesh();
}

AWildOmissionCharacter* AEquipableItem::GetOwnerCharacter() const
{
	return Cast<AWildOmissionCharacter>(GetOwner());
}

void AEquipableItem::Client_PlayThirdPersonEquipMontage_Implementation()
{
	if (GetOwnerCharacter()->IsLocallyControlled())
	{
		return;
	}

	UEquipComponent* OwnerEquipComponent = GetOwner()->FindComponentByClass<UEquipComponent>();
	if (OwnerEquipComponent == nullptr)
	{
		return;
	}

	OwnerEquipComponent->PlayEquipMontage(false);
}

void AEquipableItem::SetLocalVisibility(bool bVisible)
{
	Mesh->SetVisibility(bVisible);
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

UAnimSequence* AEquipableItem::GetEquipPose() const
{
	return EquipPose;
}

FTransform AEquipableItem::GetSocketOffset()
{
	return SocketOffset;
}

bool AEquipableItem::PrimaryEnabled() const
{
	return bPrimaryEnabled;
}

bool AEquipableItem::SecondaryEnabled() const
{
	return bSecondaryEnabled;
}