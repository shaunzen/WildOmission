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

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ItemMesh"));
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

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
	Client_AttachToPlayer();
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

AWildOmissionCharacter* AEquipableItem::GetOwnerCharacter()
{
	return Cast<AWildOmissionCharacter>(GetOwner());
}

void AEquipableItem::Client_AttachToPlayer_Implementation()
{
	AWildOmissionCharacter* OwnerCharacter = GetOwnerCharacter();

	if (OwnerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner was nullptr"));
		return;
	}

	if (HasAuthority())
	{
		AttachToComponent(OwnerCharacter->GetFirstPersonMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightHandMountSocket"));
	}
	else
	{
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightHandMountSocket"));
	}
}

void AEquipableItem::Client_PlayEquipSound_Implementation()
{
	if (GetWorld() == nullptr || EquipSound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), EquipSound, GetActorLocation());
}