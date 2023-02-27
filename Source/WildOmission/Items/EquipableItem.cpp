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

	// setup item mesh, for some reason we cant modify it from the editor
	// game seems to be crashing because no mesh is specified and we are trying to use the mesh to create a first person decoy
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = Mesh;

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

UStaticMesh* AEquipableItem::GetMesh()
{
	return Mesh->GetStaticMesh();
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
	//return Owner->GetRemoteRole() == ROLE_AutonomousProxy;
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

	AttachToComponent(OwnerCharacter->GetFirstPersonMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightHandMountSocket"));
}

void AEquipableItem::Client_PlayEquipSound_Implementation()
{
	if (GetWorld() == nullptr || EquipSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), EquipSound, GetActorLocation());
}