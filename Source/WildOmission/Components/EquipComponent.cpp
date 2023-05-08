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
	PrimaryComponentTick.bCanEverTick = true;
	
	SetIsReplicatedByDefault(true);

	FirstPersonItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("FirstPersonItemMeshComponent"));
	FirstPersonItemMeshComponent->SetCastShadow(false);

	PrimaryHeld = false;
	SecondaryHeld = false;
}

void UEquipComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEquipComponent, EquipedItem);
}

void UEquipComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (EquipedItem && PrimaryHeld)
	{
		EquipedItem->OnPrimaryHeld();
	}

	if (EquipedItem && SecondaryHeld)
	{
		EquipedItem->OnSecondaryHeld();
	}
}

void UEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AWildOmissionCharacter>(GetOwner());

	FirstPersonItemMeshComponent->AttachToComponent(OwnerCharacter->GetArmsMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("RightHandMountSocket"));

	PrimaryHeld = false;
	SecondaryHeld = false;
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

	FirstPersonArmsAnimInstance->PlayMontage(EquipedTool->GetPrimaryMontage(), EquipedTool->GetSwingSpeedRate());

	UHumanAnimInstance* ThirdPersonAnimInstance = Cast<UHumanAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if (ThirdPersonAnimInstance == nullptr)
	{
		return;
	}

	ThirdPersonAnimInstance->PlayMontage(EquipedTool->GetPrimaryMontage(), EquipedTool->GetSwingSpeedRate());
}

bool UEquipComponent::PrimaryMontagePlaying() const
{
	AToolItem* EquipedTool = Cast<AToolItem>(EquipedItem);
	if (EquipedTool == nullptr)
	{
		return false;
	}

	UHumanAnimInstance* FirstPersonArmsAnimInstance = Cast<UHumanAnimInstance>(OwnerCharacter->GetArmsMesh()->GetAnimInstance());
	if (OwnerCharacter == nullptr || FirstPersonArmsAnimInstance == nullptr)
	{
		return false;
	}

	UHumanAnimInstance* ThirdPersonAnimInstance = Cast<UHumanAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if (ThirdPersonAnimInstance == nullptr)
	{
		return false;
	}

	return FirstPersonArmsAnimInstance->Montage_IsPlaying(EquipedTool->GetPrimaryMontage()) && ThirdPersonAnimInstance->Montage_IsPlaying(EquipedTool->GetPrimaryMontage());
}

AEquipableItem* UEquipComponent::GetEquipedItem()
{
	return EquipedItem;
}

bool UEquipComponent::IsItemEquiped() const
{
	return EquipedItem != nullptr;
}

bool UEquipComponent::PrimaryEnabled() const
{
	return IsItemEquiped() && EquipedItem->PrimaryEnabled();
}

bool UEquipComponent::SecondaryEnabled() const
{
	return IsItemEquiped() && EquipedItem->SecondaryEnabled();
}

// RPC
void UEquipComponent::Server_PrimaryPressed_Implementation()
{
	PrimaryHeld = true;

	if (EquipedItem == nullptr)
	{
		return;
	}

	EquipedItem->OnPrimaryPressed();

	OnRep_EquipedItem();
}

void UEquipComponent::Server_PrimaryReleased_Implementation()
{
	if (PrimaryHeld == false)
	{
		return;
	}

	PrimaryHeld = false;

	if (EquipedItem == nullptr)
	{
		return;
	}

	EquipedItem->OnPrimaryReleased();

	OnRep_EquipedItem();
}

void UEquipComponent::Server_SecondaryPressed_Implementation()
{
	SecondaryHeld = true;

	if (EquipedItem == nullptr)
	{
		return;
	}

	EquipedItem->OnSecondaryPressed();

	OnRep_EquipedItem();
}

void UEquipComponent::Server_SecondaryReleased_Implementation()
{
	if (SecondaryHeld == false)
	{
		return;
	}

	SecondaryHeld = false;

	if (EquipedItem == nullptr)
	{
		return;
	}

	EquipedItem->OnSecondaryReleased();

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