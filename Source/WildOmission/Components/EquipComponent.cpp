// Copyright Telephone Studios. All Rights Reserved.


#include "EquipComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "WildOmission/Items/EquipableItem.h"
#include "WildOmission/Items/ToolItem.h"
#include "WildOmission/Characters/HumanAnimInstance.h"
#include "WildOmission/UI/Player/PlayerHUDWidget.h"
#include "WildOmission/UI/Inventory/PlayerInventoryWidget.h"
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

	FirstPersonItemMeshComponent->AttachToComponent(OwnerCharacter->GetArmsMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("RightHandMountSocket"));
}

void UEquipComponent::EquipItem(const FName& ItemName, TSubclassOf<AEquipableItem> Item, const int8& FromSlotIndex, const uint32& UniqueID)
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	EquipedItem = GetWorld()->SpawnActor<AEquipableItem>(Item, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation());

	EquipedItem->Equip(OwnerCharacter, ItemName, FromSlotIndex, UniqueID);

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

void UEquipComponent::DestroyEquipedItem()
{
	if (!IsItemEquiped())
	{
		return;
	}

	EquipedItem->Destroy();
	EquipedItem = nullptr;
}

void UEquipComponent::PlayPrimaryMontage()
{
	AToolItem* EquipedTool = Cast<AToolItem>(EquipedItem);
	if (EquipedTool == nullptr)
	{
		return;
	}

	UHumanAnimInstance* FirstPersonArmsAnimInstance = Cast<UHumanAnimInstance>(OwnerCharacter->GetArmsMesh()->GetAnimInstance());
	if (OwnerCharacter == nullptr || FirstPersonArmsAnimInstance == nullptr)
	{
		return;
	}

	FirstPersonArmsAnimInstance->PlayMontage(EquipedTool->GetPrimaryMontage());

	UHumanAnimInstance* ThirdPersonAnimInstance = Cast<UHumanAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if (ThirdPersonAnimInstance == nullptr)
	{
		return;
	}

	ThirdPersonAnimInstance->PlayMontage(EquipedTool->GetPrimaryMontage());
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

	OnRep_EquipedItem();
}

void UEquipComponent::Server_Secondary_Implementation()
{
	if (EquipedItem == nullptr)
	{
		return;
	}

	EquipedItem->Secondary();

	OnRep_EquipedItem();
}

void UEquipComponent::OnRep_EquipedItem()
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	if (EquipedItem)
	{
		RefreshEquipedSlot();

		FirstPersonItemMeshComponent->SetStaticMesh(EquipedItem->GetMesh());

		FirstPersonItemMeshComponent->SetVisibility(OwnerCharacter->IsLocallyControlled());
		FirstPersonItemMeshComponent->SetRelativeLocation(EquipedItem->GetSocketOffset().GetLocation());
		FirstPersonItemMeshComponent->SetRelativeRotation(EquipedItem->GetSocketOffset().GetRotation());

		EquipedItem->SetLocalVisibility(!OwnerCharacter->IsLocallyControlled());
	}
	else
	{
		FirstPersonItemMeshComponent->SetVisibility(false);
	}

}

void UEquipComponent::RefreshEquipedSlot()
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	UPlayerHUDWidget* PlayerHUD = OwnerCharacter->GetHUDWidget();
	if (PlayerHUD == nullptr)
	{
		return;
	}
	
	UPlayerInventoryWidget* PlayerInventoryWidget = PlayerHUD->GetPlayerInventoryWidget();
	if (PlayerInventoryWidget == nullptr)
	{
		return;
	}

	PlayerInventoryWidget->RefreshSlot(EquipedItem->GetFromSlotIndex());
}