// Copyright Telephone Studios. All Rights Reserved.


#include "EquipableItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
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

	ConstructorHelpers::FObjectFinder<USoundBase> EquipSoundObject(TEXT("/Game/WildOmission/Items/EquipableItems/Audio/EquipDefault/Equip_Default_Cue"));
	ConstructorHelpers::FObjectFinder<UAnimSequence> EquipPoseObject(TEXT("/Game/WildOmission/Characters/Human/Animation/Items/A_Human_HoldTool"));

	if (EquipSoundObject.Object == nullptr || EquipPoseObject.Object == nullptr)
	{
		return;
	}

	EquipSound = EquipSoundObject.Object;
	EquipPose = EquipPoseObject.Object;

	FromSlotIndex = -1;
	UniqueID = 0;
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

UStaticMesh* AEquipableItem::GetMesh()
{
	return Mesh->GetStaticMesh();
}

AWildOmissionCharacter* AEquipableItem::GetOwnerCharacter() const
{
	return Cast<AWildOmissionCharacter>(GetOwner());
}

void AEquipableItem::Client_PlayEquipSound_Implementation()
{
	if (GetWorld() == nullptr || EquipSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), EquipSound, GetActorLocation());
}

void AEquipableItem::SetLocalVisibility(bool bVisible)
{
	Mesh->SetVisibility(bVisible);
}

int8 AEquipableItem::GetFromSlotIndex() const
{
	return FromSlotIndex;
}

uint32 AEquipableItem::GetUniqueItemID() const
{
	return UniqueID;
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