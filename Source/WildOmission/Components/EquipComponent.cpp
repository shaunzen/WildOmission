// Copyright Telephone Studios. All Rights Reserved.


#include "EquipComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Items/EquipableItem.h"
#include "WildOmission/Characters/HumanAnimInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UEquipComponent::UEquipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	SetIsReplicatedByDefault(true);

	FirstPersonItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("FirstPersonItemMeshComponent"));
	FirstPersonItemMeshComponent->SetCastShadow(false);
}

void UEquipComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEquipComponent, EquipedItem);
}

void UEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AWildOmissionCharacter>(GetOwner());

	FirstPersonItemMeshComponent->AttachToComponent(OwnerCharacter->GetArmsMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightHandMountSocket"));
}

void UEquipComponent::EquipItem(TSubclassOf<AEquipableItem> Item)
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	EquipedItem = GetWorld()->SpawnActor<AEquipableItem>(Item, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation());

	EquipedItem->Equip(OwnerCharacter);
	
	if (GetOwner()->HasAuthority())
	{
		OnRep_EquipedItem();
	}
}

void UEquipComponent::Disarm()
{
	if (!IsItemEquiped())
	{
		return;
	}

	EquipedItem->OnUnequip();
	EquipedItem->Destroy();

	EquipedItem = nullptr;

	if (GetOwner()->HasAuthority())
	{
		OnRep_EquipedItem();
	}
}

void UEquipComponent::PlaySwingAnimation()
{
	UHumanAnimInstance* FirstPersonArmsAnimInstance = Cast<UHumanAnimInstance>(OwnerCharacter->GetArmsMesh()->GetAnimInstance());
	if (OwnerCharacter == nullptr || FirstPersonArmsAnimInstance == nullptr)
	{
		return;
	}

	FirstPersonArmsAnimInstance->PlaySwingAnimation();

	UHumanAnimInstance* ThirdPersonAnimInstance = Cast<UHumanAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if (ThirdPersonAnimInstance == nullptr)
	{
		return;
	}

	ThirdPersonAnimInstance->PlaySwingAnimation();
}

AEquipableItem* UEquipComponent::GetEquipedItem()
{
	return EquipedItem;
}

bool UEquipComponent::IsItemEquiped() const
{
	return EquipedItem != nullptr;
}

// RPC
void UEquipComponent::Server_Primary_Implementation()
{
	if (EquipedItem == nullptr)
	{
		return;
	}

	EquipedItem->Primary();
}

void UEquipComponent::Server_Secondary_Implementation()
{
	if (EquipedItem == nullptr)
	{
		return;
	}

	EquipedItem->Secondary();
}

void UEquipComponent::OnRep_EquipedItem()
{
	if (EquipedItem)
	{
		FirstPersonItemMeshComponent->SetStaticMesh(EquipedItem->GetMesh());

		FirstPersonItemMeshComponent->SetVisibility(OwnerCharacter->IsLocallyControlled());

		EquipedItem->SetLocalVisibility(!OwnerCharacter->IsLocallyControlled());
	}
	else
	{
		FirstPersonItemMeshComponent->SetVisibility(false);
	}
}