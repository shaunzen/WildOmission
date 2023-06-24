// Copyright Telephone Studios. All Rights Reserved.


#include "Components/EquipComponent.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"
#include "Items/EquipableItem.h"
#include "Items/ToolItem.h"
#include "WildOmission/Characters/HumanAnimInstance.h"
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
	
	if (IsEquipedItemValid() && PrimaryHeld)
	{
		EquipedItem->OnPrimaryHeld();
	}

	if (IsEquipedItemValid()  && SecondaryHeld)
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
		EquipFirstPersonViewModel(ItemClass, UniqueID);
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
	if (OwnerCharacter && OwnerCharacter->IsLocallyControlled())
	{
		EquipFirstPersonViewModel(nullptr, 0);
	}

	if (GetOwner()->HasAuthority() && IsItemEquiped())
	{
		EquipedItem->OnUnequip();
		DestroyEquipedItem();

		OnRep_EquipedItem();
	}
}

void UEquipComponent::DestroyEquipedItem()
{
	if (EquipedItem == nullptr)
	{
		return;
	}

	EquipedItem->Destroy();
	EquipedItem = nullptr;
}

void UEquipComponent::PlayEquipMontage(bool FirstPerson)
{
	if (FirstPerson && LocalEquipedItemDefaultClass)
	{
		UHumanAnimInstance* FirstPersonArmsAnimInstance = Cast<UHumanAnimInstance>(OwnerCharacter->GetArmsMesh()->GetAnimInstance());
		if (OwnerCharacter == nullptr || FirstPersonArmsAnimInstance == nullptr)
		{
			return;
		}

		FirstPersonArmsAnimInstance->PlayMontage(LocalEquipedItemDefaultClass->GetEquipMontage());
	}
	else if (!FirstPerson && EquipedItem)
	{
		UHumanAnimInstance* ThirdPersonAnimInstance = Cast<UHumanAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
		if (ThirdPersonAnimInstance == nullptr)
		{
			return;
		}

		ThirdPersonAnimInstance->PlayMontage(EquipedItem->GetEquipMontage());
	}
}

void UEquipComponent::PlayPrimaryMontage(bool FirstPerson)
{
	AToolItem* EquipedTool = Cast<AToolItem>(EquipedItem);
	if (EquipedTool == nullptr)
	{
		return;
	}

	if (FirstPerson)
	{
		UHumanAnimInstance* FirstPersonArmsAnimInstance = Cast<UHumanAnimInstance>(OwnerCharacter->GetArmsMesh()->GetAnimInstance());
		if (OwnerCharacter == nullptr || FirstPersonArmsAnimInstance == nullptr)
		{
			return;
		}

		FirstPersonArmsAnimInstance->PlayMontage(EquipedTool->GetPrimaryMontage(), EquipedTool->GetSwingSpeedRate());
	}
	else
	{
		UHumanAnimInstance* ThirdPersonAnimInstance = Cast<UHumanAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
		if (ThirdPersonAnimInstance == nullptr)
		{
			return;
		}

		ThirdPersonAnimInstance->PlayMontage(EquipedTool->GetPrimaryMontage(), EquipedTool->GetSwingSpeedRate());
	}
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

	return FirstPersonArmsAnimInstance->Montage_IsPlaying(EquipedTool->GetPrimaryMontage()) || ThirdPersonAnimInstance->Montage_IsPlaying(EquipedTool->GetPrimaryMontage());
}

AEquipableItem* UEquipComponent::GetEquipedItem()
{
	return EquipedItem;
}

AEquipableItem* UEquipComponent::GetLocalEquipedItemDefaultClass()
{
	return LocalEquipedItemDefaultClass;
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

void UEquipComponent::PrimaryPressed()
{
	if (!GetOwner()->HasAuthority())
	{
		Server_PrimaryPressed();
	}

	PrimaryHeld = true;

	if (!IsEquipedItemValid())
	{
		return;
	}

	EquipedItem->OnPrimaryPressed();
}

void UEquipComponent::PrimaryReleased()
{
	if (!GetOwner()->HasAuthority())
	{
		Server_PrimaryReleased();
	}

	if (PrimaryHeld == false)
	{
		return;
	}

	PrimaryHeld = false;

	if (!IsEquipedItemValid())
	{
		return;
	}

	EquipedItem->OnPrimaryReleased();
}

void UEquipComponent::SecondaryPressed()
{
	if (!GetOwner()->HasAuthority())
	{
		Server_SecondaryPressed();
	}

	SecondaryHeld = true;

	if (!IsEquipedItemValid())
	{
		return;
	}

	EquipedItem->OnSecondaryPressed();
}

void UEquipComponent::SecondaryReleased()
{
	if (!GetOwner()->HasAuthority())
	{
		Server_SecondaryReleased();
	}

	if (SecondaryHeld == false)
	{
		return;
	}

	SecondaryHeld = false;
	
	if (!IsEquipedItemValid())
	{
		return;
	}

	EquipedItem->OnSecondaryReleased();
}

void UEquipComponent::OnRep_EquipedItem()
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	if (EquipedItem)
	{
		RefreshEquipedSlotUI();
		
		EquipedItem->SetLocalVisibility(!OwnerCharacter->IsLocallyControlled());

		PlayEquipMontage(false);
	}
}

void UEquipComponent::EquipFirstPersonViewModel(TSubclassOf<AEquipableItem> ItemClass, const uint32& UniqueID)
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	if (ItemClass != nullptr)
	{
		LocalEquipedItemDefaultClass = ItemClass.GetDefaultObject();
		LocalEquipedItemDefaultClass->SetUniqueItemID(UniqueID);

		if (LocalEquipedItemDefaultClass == nullptr)
		{
			return;
		}

		FirstPersonItemMeshComponent->SetStaticMesh(LocalEquipedItemDefaultClass->GetMesh());

		FirstPersonItemMeshComponent->SetVisibility(OwnerCharacter->IsLocallyControlled());
		FirstPersonItemMeshComponent->SetRelativeLocation(LocalEquipedItemDefaultClass->GetSocketOffset().GetLocation());
		FirstPersonItemMeshComponent->SetRelativeRotation(LocalEquipedItemDefaultClass->GetSocketOffset().GetRotation());

		PlayEquipMontage(true);
	}
	else
	{
		LocalEquipedItemDefaultClass = nullptr;
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

bool UEquipComponent::IsEquipedItemValid() const
{
	if (OwnerCharacter->IsLocallyControlled())
	{
		return EquipedItem != nullptr && LocalEquipedItemDefaultClass != nullptr && EquipedItem->GetClass() == LocalEquipedItemDefaultClass->GetClass();
	}

	return EquipedItem != nullptr;
}

//******************************************
//	RPC
//******************************************

void UEquipComponent::Server_PrimaryPressed_Implementation()
{
	PrimaryPressed();
}

void UEquipComponent::Server_PrimaryReleased_Implementation()
{
	PrimaryReleased();
}

void UEquipComponent::Server_SecondaryPressed_Implementation()
{
	SecondaryPressed();
}

void UEquipComponent::Server_SecondaryReleased_Implementation()
{
	SecondaryReleased();
}