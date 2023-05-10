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

void UEquipComponent::EquipItem(const FName& ItemName, TSubclassOf<AEquipableItem> ItemClass, const int8& FromSlotIndex, const uint32& UniqueID)
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	if (OwnerCharacter->IsLocallyControlled())
	{
		EquipFirstPersonViewModel(ItemClass);
	}

	if (GetOwner()->HasAuthority())
	{
		EquipedItem = GetWorld()->SpawnActor<AEquipableItem>(ItemClass, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation());

		EquipedItem->Equip(OwnerCharacter, ItemName, FromSlotIndex, UniqueID);

		OnRep_EquipedItem();
	}
}

void UEquipComponent::Disarm()
{
	if (!IsItemEquiped())
	{
		return;
	}

	if (OwnerCharacter && OwnerCharacter->IsLocallyControlled())
	{
		EquipFirstPersonViewModel(nullptr);
	}

	if (GetOwner()->HasAuthority())
	{
		EquipedItem->OnUnequip();
		DestroyEquipedItem();

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

void UEquipComponent::PlayEquipMontage()
{
	if (EquipedItem == nullptr)
	{
		return;
	}


	UHumanAnimInstance* FirstPersonArmsAnimInstance = Cast<UHumanAnimInstance>(OwnerCharacter->GetArmsMesh()->GetAnimInstance());
	if (OwnerCharacter == nullptr || FirstPersonArmsAnimInstance == nullptr)
	{
		return;
	}

	FirstPersonArmsAnimInstance->PlayMontage(EquipedItem->GetEquipMontage());

	UHumanAnimInstance* ThirdPersonAnimInstance = Cast<UHumanAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if (ThirdPersonAnimInstance == nullptr)
	{
		return;
	}

	ThirdPersonAnimInstance->PlayMontage(EquipedItem->GetEquipMontage());
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
	AToolItem* EquipedTool = Cast<AToolItem>(LocallyEquipedItem);
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

	return FirstPersonArmsAnimInstance->Montage_IsPlaying(EquipedTool->GetPrimaryMontage()) || ThirdPersonAnimInstance->Montage_IsPlaying(EquipedTool->GetPrimaryMontage());
}

AEquipableItem* UEquipComponent::GetLocallyEquipedItem()
{
	return LocallyEquipedItem;
}

bool UEquipComponent::IsItemEquiped() const
{
	return LocallyEquipedItem != nullptr;
}

bool UEquipComponent::PrimaryEnabled() const
{
	return IsItemEquiped() && LocallyEquipedItem->PrimaryEnabled();
}

bool UEquipComponent::SecondaryEnabled() const
{
	return IsItemEquiped() && LocallyEquipedItem->SecondaryEnabled();
}

// RPC

void UEquipComponent::OnRep_EquipedItem()
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	if (EquipedItem)
	{
		RefreshEquipedSlotUI();
		
		EquipFirstPersonViewModel(EquipedItem->GetClass());

		EquipedItem->SetLocalVisibility(!OwnerCharacter->IsLocallyControlled());
	}
	else
	{
		EquipFirstPersonViewModel(nullptr);
	}
}

void UEquipComponent::EquipFirstPersonViewModel(TSubclassOf<AEquipableItem> ItemClass)
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	if (ItemClass != nullptr)
	{
		LocallyEquipedItem = ItemClass.GetDefaultObject();
		if (LocallyEquipedItem == nullptr)
		{
			return;
		}

		FirstPersonItemMeshComponent->SetStaticMesh(LocallyEquipedItem->GetMesh());

		FirstPersonItemMeshComponent->SetVisibility(OwnerCharacter->IsLocallyControlled());
		FirstPersonItemMeshComponent->SetRelativeLocation(LocallyEquipedItem->GetSocketOffset().GetLocation());
		FirstPersonItemMeshComponent->SetRelativeRotation(LocallyEquipedItem->GetSocketOffset().GetRotation());

		// play equip animation locally
	}
	else
	{
		LocallyEquipedItem = nullptr;
		FirstPersonItemMeshComponent->SetVisibility(false);
	}
}

void UEquipComponent::RefreshEquipedSlotUI()
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