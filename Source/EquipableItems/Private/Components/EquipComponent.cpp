// Copyright Telephone Studios. All Rights Reserved.


#include "Components/EquipComponent.h"
#include "Components/PlayerInventoryComponent.h"
#include "Items/EquipableItem.h"
#include "Items/ToolItem.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UEquipComponent::UEquipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	SetIsReplicatedByDefault(true);

	FirstPersonItemComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonItemComponent"));
	FirstPersonItemComponent->SetAnimInstanceClass(UAnimInstance::StaticClass());
	FirstPersonItemComponent->SetCastShadow(false);
	
	PrimaryHeld = false;
	SecondaryHeld = false;
	EquipedItem = nullptr;
	LocalEquipedItemDefaultClass = nullptr;

	OwnerPawn = nullptr;
	OwnerFirstPersonMesh = nullptr;
	OwnerThirdPersonMesh = nullptr;
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

void UEquipComponent::Setup(USkeletalMeshComponent* FirstPersonMeshComponent, USkeletalMeshComponent* ThirdPersonMeshComponent)
{
	OwnerFirstPersonMesh = FirstPersonMeshComponent;
	OwnerThirdPersonMesh = ThirdPersonMeshComponent;
	FirstPersonItemComponent->AttachToComponent(FirstPersonMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("RightHandMountSocket"));
}

void UEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPawn = Cast<APawn>(GetOwner());
	
	PrimaryHeld = false;
	SecondaryHeld = false;

	UPlayerInventoryComponent* OwnerInventory = GetOwner()->FindComponentByClass<UPlayerInventoryComponent>();
	if (OwnerInventory == nullptr)
	{
		return;
	}

	OwnerInventory->OnToolbarSlotSelectionChange.AddDynamic(this, &UEquipComponent::RefreshEquip);
}

void UEquipComponent::EquipItem(const FName& ItemName, TSubclassOf<AEquipableItem> ItemClass, const int8& FromSlotIndex, const uint32& UniqueID)
{
	if (OwnerPawn == nullptr)
	{
		return;
	}

	if (OwnerPawn->IsLocallyControlled() && OwnerPawn->GetController()->IsPlayerController())
	{
		EquipFirstPersonViewModel(ItemClass, UniqueID);
	}

	if (GetOwner()->HasAuthority())
	{	
		EquipedItem = GetWorld()->SpawnActor<AEquipableItem>(ItemClass, OwnerPawn->GetActorLocation(), OwnerPawn->GetActorRotation());

		EquipedItem->Equip(OwnerPawn, OwnerThirdPersonMesh, ItemName, FromSlotIndex, UniqueID);

		OnRep_EquipedItem();
	}
}

void UEquipComponent::Disarm()
{
	if (OwnerPawn && OwnerPawn->IsLocallyControlled() && OwnerPawn->GetController()->IsPlayerController())
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

void UEquipComponent::UpdateControlRotation(const FRotator& NewControlRotation)
{
	OwnerReplicatedControlRotation = NewControlRotation;
}

FRotator UEquipComponent::GetOwnerControlRotation() const
{
	return OwnerReplicatedControlRotation;
}

USkeletalMeshComponent* UEquipComponent::GetFirstPersonItemComponent() const
{
	return FirstPersonItemComponent;
}

void UEquipComponent::PlayMontage(UAnimMontage* Montage)
{
	if (OwnerPawn == nullptr)
	{
		return;
	}
	
	const bool UseFirstPersonInstance = OwnerPawn->IsLocallyControlled() && OwnerPawn->GetController()->IsPlayerController();

	if (UseFirstPersonInstance && LocalEquipedItemDefaultClass)
	{
		UAnimInstance* FirstPersonArmsAnimInstance = OwnerFirstPersonMesh->GetAnimInstance();
		if (FirstPersonArmsAnimInstance == nullptr)
		{
			return;
		}

		FirstPersonArmsAnimInstance->Montage_Play(Montage);
	}
	else if (!UseFirstPersonInstance && EquipedItem)
	{
		UAnimInstance* ThirdPersonAnimInstance = OwnerThirdPersonMesh->GetAnimInstance();
		if (ThirdPersonAnimInstance == nullptr)
		{
			return;
		}

		ThirdPersonAnimInstance->Montage_Play(Montage);
	}

	if (GetOwner()->HasAuthority())
	{
		Multi_PlayMontage(Montage);
	}
}

void UEquipComponent::PlayItemMontage(UAnimMontage* Montage)
{
	if (OwnerPawn == nullptr)
	{
		return;
	}

	const bool UseFirstPersonInstance = OwnerPawn->IsLocallyControlled() && OwnerPawn->GetController()->IsPlayerController();
	
	if (UseFirstPersonInstance && LocalEquipedItemDefaultClass)
	{
		UAnimInstance* FirstPersonItemAnimInstance = FirstPersonItemComponent->GetAnimInstance();
		if (FirstPersonItemAnimInstance == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("FirstPersonItemAnimInstance nullptr."));
			return;
		}

		FirstPersonItemAnimInstance->Montage_Play(Montage);
	}
	else if (!UseFirstPersonInstance && EquipedItem)
	{
		UAnimInstance* ThirdPersonItemAnimInstance = EquipedItem->GetMeshComponent()->GetAnimInstance();
		if (ThirdPersonItemAnimInstance == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("ThirdPersonItemAnimInstance nullptr."));
			return;
		}

		ThirdPersonItemAnimInstance->Montage_Play(Montage);
	}

	if (GetOwner()->HasAuthority())
	{
		Multi_PlayItemMontage(Montage);
	}
}

bool UEquipComponent::IsMontagePlaying(UAnimMontage* Montage) const
{
	return IsFirstPersonMontagePlaying(Montage) || IsThirdPersonMontagePlaying(Montage);
}

bool UEquipComponent::IsItemMontagePlaying(UAnimMontage* Montage) const
{
	UAnimInstance* FirstPersonItemAnimInstance = FirstPersonItemComponent->GetAnimInstance();
	if (FirstPersonItemAnimInstance == nullptr || EquipedItem == nullptr)
	{
		return false;
	}

	UAnimInstance* ThirdPersonItemAnimInstance = EquipedItem->GetMeshComponent()->GetAnimInstance();
	if (ThirdPersonItemAnimInstance == nullptr)
	{
		return false;
	}

	return FirstPersonItemAnimInstance->Montage_IsPlaying(Montage) || ThirdPersonItemAnimInstance->Montage_IsPlaying(Montage);
}

void UEquipComponent::OnPrimaryAnimationClimax(bool FirstPerson)
{
	if (EquipedItem == nullptr)
	{
		return;
	}

	EquipedItem->OnPrimaryAnimationClimax(FirstPerson);
}

void UEquipComponent::OnSecondaryAnimationClimax(bool FirstPerson)
{
	if (EquipedItem == nullptr)
	{
		return;
	}

	EquipedItem->OnSecondaryAnimationClimax(FirstPerson);
}

void UEquipComponent::OnReloadAnimationClimax(bool FirstPerson)
{
	if (EquipedItem == nullptr)
	{
		return;
	}

	EquipedItem->OnReloadAnimationClimax(FirstPerson);
}

AEquipableItem* UEquipComponent::GetEquipedItem()
{
	return EquipedItem;
}

UAnimSequence* UEquipComponent::GetEquipedItemPose()
{
	if (OwnerPawn == nullptr)
	{
		return nullptr;
	}

	if (OwnerPawn->IsLocallyControlled() && OwnerPawn->GetController()->IsPlayerController())
	{
		if (LocalEquipedItemDefaultClass == nullptr)
		{
			return nullptr;
		}

		return LocalEquipedItemDefaultClass->GetEquipPose();
	}
	else
	{
		if (EquipedItem == nullptr)
		{
			return nullptr;
		}

		return EquipedItem->GetEquipPose();
	}
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

void UEquipComponent::ReloadPressed()
{
	if (!GetOwner()->HasAuthority())
	{
		Server_ReloadPressed();
	}

	if (!IsEquipedItemValid() || PrimaryHeld || SecondaryHeld)
	{
		return;
	}

	EquipedItem->OnReloadPressed();
}

void UEquipComponent::OnRep_EquipedItem()
{
	if (OwnerPawn == nullptr)
	{
		return;
	}

	if (EquipedItem)
	{
		RefreshEquipedSlotUI.Broadcast(EquipedItem->GetFromSlotIndex());
		
		EquipedItem->SetLocalVisibility(!OwnerPawn->IsLocallyControlled());

		PlayMontage(EquipedItem->GetEquipMontage());
		if (EquipedItem->GetEquipItemMontage() != nullptr)
		{
			PlayItemMontage(EquipedItem->GetEquipItemMontage());
		}
	}
}

void UEquipComponent::EquipFirstPersonViewModel(TSubclassOf<AEquipableItem> ItemClass, const uint32& UniqueID)
{
	if (OwnerPawn == nullptr)
	{
		return;
	}

	if (ItemClass != nullptr)
	{
		LocalEquipedItemDefaultClass = ItemClass.GetDefaultObject();
		LocalEquipedItemDefaultClass->SetUniqueItemID(UniqueID);

		if (LocalEquipedItemDefaultClass == nullptr || OwnerFirstPersonMesh == nullptr)
		{
			return;
		}

		LocalEquipedItemDefaultClass->IsLeftHandMounted() ?
		FirstPersonItemComponent->AttachToComponent(OwnerFirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("LeftHandMountSocket")) :
		FirstPersonItemComponent->AttachToComponent(OwnerFirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("RightHandMountSocket"));

		FirstPersonItemComponent->SetSkeletalMeshAsset(LocalEquipedItemDefaultClass->GetMeshComponent()->GetSkeletalMeshAsset());
		FirstPersonItemComponent->SetAnimInstanceClass(LocalEquipedItemDefaultClass->GetMeshComponent()->GetAnimClass());
		
		FirstPersonItemComponent->SetVisibility(OwnerPawn->IsLocallyControlled());
		FirstPersonItemComponent->SetRelativeLocation(LocalEquipedItemDefaultClass->GetSocketOffset().GetLocation());
		FirstPersonItemComponent->SetRelativeRotation(LocalEquipedItemDefaultClass->GetSocketOffset().GetRotation());

		PlayMontage(LocalEquipedItemDefaultClass->GetEquipMontage());

		if (LocalEquipedItemDefaultClass->GetEquipItemMontage() != nullptr)
		{
			PlayItemMontage(LocalEquipedItemDefaultClass->GetEquipItemMontage());
		}
	}
	else
	{
		LocalEquipedItemDefaultClass = nullptr;
		FirstPersonItemComponent->SetVisibility(false);
	}
}

void UEquipComponent::RefreshEquip(const int8& NewSlotIndex, const FInventorySlot& NewSlot)
{
	if (OwnerPawn == nullptr)
	{
		Disarm();
		return;
	}

	// return if there is no item
	if (NewSlot.IsEmpty())
	{
		Disarm();
		return;
	}

	// get the equipable subclass for this item
	FItemData* SlotItemData = UInventoryComponent::GetItemData(NewSlot.Item.Name);
	if (SlotItemData == nullptr || SlotItemData->EquipItemClass == nullptr)
	{
		Disarm();
		return;
	}

	AEquipableItem* CurrentEquipedItem = EquipedItem;
	AEquipableItem* CurrentEquipedDefaultClass = LocalEquipedItemDefaultClass;

	// is this item the same as we are already holding
	if (GetOwner()->HasAuthority() && CurrentEquipedItem && SlotItemData->EquipItemClass.Get() == CurrentEquipedItem->GetClass() && NewSlot.Item.UniqueID == CurrentEquipedItem->GetUniqueItemID())
	{
		return;
	}

	// Locallized version of same item check
	if (OwnerPawn->IsLocallyControlled() && CurrentEquipedDefaultClass && SlotItemData->EquipItemClass.Get() == CurrentEquipedDefaultClass->GetClass() && NewSlot.Item.UniqueID == CurrentEquipedDefaultClass->GetUniqueItemID())
	{
		return;
	}

	Disarm();
	EquipItem(NewSlot.Item.Name, SlotItemData->EquipItemClass.Get(), NewSlotIndex, NewSlot.Item.UniqueID);

}

bool UEquipComponent::IsEquipedItemValid() const
{
	if (OwnerPawn->IsLocallyControlled() && OwnerPawn->GetController()->IsPlayerController())
	{
		return EquipedItem != nullptr && LocalEquipedItemDefaultClass != nullptr && EquipedItem->GetClass() == LocalEquipedItemDefaultClass->GetClass();
	}

	return EquipedItem != nullptr;
}

bool UEquipComponent::IsFirstPersonMontagePlaying(UAnimMontage* Montage) const
{
	if (OwnerFirstPersonMesh == nullptr)
	{
		return false;
	}

	UAnimInstance* FirstPersonArmsAnimInstance = OwnerFirstPersonMesh->GetAnimInstance();
	if (FirstPersonArmsAnimInstance == nullptr)
	{
		return false;
	}

	return FirstPersonArmsAnimInstance->Montage_IsPlaying(Montage);
}

bool UEquipComponent::IsThirdPersonMontagePlaying(UAnimMontage* Montage) const
{
	if (OwnerThirdPersonMesh == nullptr)
	{
		return false;
	}
	
	UAnimInstance* ThirdPersonAnimInstance = OwnerThirdPersonMesh->GetAnimInstance();
	if (ThirdPersonAnimInstance == nullptr)
	{
		return false;
	}

	return ThirdPersonAnimInstance->Montage_IsPlaying(Montage);
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

void UEquipComponent::Server_ReloadPressed_Implementation()
{
	ReloadPressed();
}

bool UEquipComponent::MulticastMontageConditionsValid() const
{
	if (OwnerPawn == nullptr || OwnerPawn->GetController() == nullptr)
	{
		return false;
	}

	if (OwnerPawn->IsLocallyControlled() && OwnerPawn->IsPlayerControlled())
	{
		return false;
	}

	if (OwnerPawn->HasAuthority())
	{
		return false;
	}

	return true;
}

void UEquipComponent::Multi_PlayMontage_Implementation(UAnimMontage* Montage)
{
	if (!MulticastMontageConditionsValid())
	{
		return;
	}

	PlayMontage(Montage);
}

void UEquipComponent::Multi_PlayItemMontage_Implementation(UAnimMontage* Montage)
{
	if (!MulticastMontageConditionsValid())
	{
		return;
	}

	PlayMontage(Montage);
}
