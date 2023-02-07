// Copyright Telephone Studios. All Rights Reserved.


#include "EquipableItem.h"
#include "WildOmission/Player/WildOmissionCharacter.h"

// Sets default values
AEquipableItem::AEquipableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ItemMesh"));
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;
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
	OwnerCharacter = InOwnerCharacter;
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, FString::Printf(TEXT("Equip action and my owner is: %s"), *OwnerCharacter->GetActorNameOrLabel()));

}

void AEquipableItem::Primary()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, FString::Printf(TEXT("Primary action and my owner is: %s"), *OwnerCharacter->GetActorNameOrLabel()));
}

void AEquipableItem::Secondary()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, FString::Printf(TEXT("Secondary action and my owner is: %s"), *OwnerCharacter->GetActorNameOrLabel()));
}