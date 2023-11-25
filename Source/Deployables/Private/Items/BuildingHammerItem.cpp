// Copyright Telephone Studios. All Rights Reserved.


#include "Items/BuildingHammerItem.h"
#include "Components/EquipComponent.h"
#include "Components/BuilderComponent.h"
#include "Interfaces/DurabilityInterface.h"
#include "Components/InventoryComponent.h"
#include "UI/BuildingHammerWidget.h"
#include "Deployables/Deployable.h"
#include "Deployables/BuildingBlock.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DamageEvents.h"
#include "UObject/ConstructorHelpers.h"

static TSubclassOf<UBuildingHammerWidget> WidgetClass = nullptr;

ABuildingHammerItem::ABuildingHammerItem()
{
	DealDamageToActors = false;

	ToolType = BUILD;
	EffectiveRangeCentimeters = 300.0f;
	MaxRepairAmount = 15.0f;

	Widget = nullptr;

	static ConstructorHelpers::FClassFinder<UBuildingHammerWidget> BuildingHammerWidgetBlueprint(TEXT("/Game/Deployables/UI/WBP_BuildingHammerMenu"));
	if (BuildingHammerWidgetBlueprint.Succeeded())
	{
		WidgetClass = BuildingHammerWidgetBlueprint.Class;
	}
}

void ABuildingHammerItem::OnPrimaryHeld()
{
	Super::OnPrimaryHeld();

	Swing();
}

void ABuildingHammerItem::OnSecondaryPressed()
{
	Super::OnSecondaryPressed();

	if (GetOwnerPawn() == nullptr || GetOwnerPawn()->IsLocallyControlled() == false)
	{
		return;
	}

	APlayerController* OwnerPlayerController = Cast<APlayerController>(GetOwnerPawn()->GetController());
	if (OwnerPlayerController == nullptr)
	{
		return;
	}

	FHitResult HitResult;
	if (!LineTraceOnVisibility(HitResult))
	{
		return;
	}

	ADeployable* HitDeployable = Cast<ADeployable>(HitResult.GetActor());
	if (HitDeployable == nullptr || !HasAuthorization(HitDeployable->GetActorLocation()))
	{
		return;
	}

	if (Widget)
	{
		Widget->Teardown();
	}

	Widget = CreateWidget<UBuildingHammerWidget>(OwnerPlayerController, WidgetClass);
	if (Widget == nullptr)
	{
		return;
	}
	Widget->OnTeardown.AddDynamic(this, &ABuildingHammerItem::ClearWidget);
	Widget->Show(this, HitDeployable);
}


void ABuildingHammerItem::OnUnequip()
{
	Super::OnUnequip();

	if (Widget)
	{
		Widget->Teardown();
	}
}

void ABuildingHammerItem::Server_UpgradeCurrentDeployable_Implementation()
{
	FHitResult HitResult;
	if (!LineTraceOnVisibility(HitResult))
	{
		return;
	}

	ABuildingBlock* HitBuildingBlock = Cast<ABuildingBlock>(HitResult.GetActor());
	if (HitBuildingBlock == nullptr || !HitBuildingBlock->IsUpgradable() || !HasAuthorization(HitBuildingBlock->GetActorLocation()))
	{
		return;
	}

	UInventoryComponent* OwnerInventoryComponent = Cast<UInventoryComponent>(GetOwner()->FindComponentByClass<UInventoryComponent>());
	if (OwnerInventoryComponent == nullptr)
	{
		return;
	}
	FInventoryItem UpgradeCost = GetUpgradeCostForBuildingBlock(HitBuildingBlock);
	if (OwnerInventoryComponent->GetContents()->GetItemQuantity(UpgradeCost.Name) < UpgradeCost.Quantity)
	{
		return;
	}

	OwnerInventoryComponent->RemoveItem(UpgradeCost);

	HitBuildingBlock->Upgrade();
	DecrementDurability();
}

void ABuildingHammerItem::Server_DestroyCurrentDeployable_Implementation()
{
	FHitResult HitResult;
	if (!LineTraceOnVisibility(HitResult))
	{
		return;
	}

	ADeployable* HitDeployable = Cast<ADeployable>(HitResult.GetActor());
	if (HitDeployable == nullptr || !HasAuthorization(HitDeployable->GetActorLocation()))
	{
		return;
	}

	UInventoryComponent* OwnerInventoryComponent = GetOwner()->FindComponentByClass<UInventoryComponent>();
	if (OwnerInventoryComponent == nullptr)
	{
		return;
	}

	FInventoryItem Refund = GetDestructionRefundForDeployable(HitDeployable);
	OwnerInventoryComponent->AddItem(Refund);

	HitDeployable->Destroy();
	DecrementDurability();
}

bool ABuildingHammerItem::GetLookingAtItemDurability(float& OutCurrentDurability, float& OutMaxDurability, FString& OutActorName) const
{
	if (GetOwnerPawn() == nullptr)
	{
		return false;
	}
	
	FHitResult HitResult;
	if (!LineTraceOnVisibility(HitResult))
	{
		return false;
	}

	IDurabilityInterface* DurabilityInterfaceActor = Cast<IDurabilityInterface>(HitResult.GetActor());
	if (DurabilityInterfaceActor == nullptr)
	{
		return false;
	}
	
	OutCurrentDurability = DurabilityInterfaceActor->GetCurrentDurability();
	OutMaxDurability = DurabilityInterfaceActor->GetMaxDurability();
	OutActorName = HitResult.GetActor()->GetActorNameOrLabel();
	return true;
}

FName ABuildingHammerItem::GetResourceIDFromMaterialType(TEnumAsByte<EToolType> MaterialType)
{
	FName ResourceID = NAME_None;

	switch (MaterialType)
	{
	case EToolType::WOOD:
		ResourceID = TEXT("wood");
		break;
	case EToolType::STONE:
		ResourceID = TEXT("stone");
		break;
	case EToolType::METAL:
		ResourceID = TEXT("metal");
		break;
	default:
		ResourceID = TEXT("wood");
		break;
	}

	return ResourceID;
}

FInventoryItem ABuildingHammerItem::GetUpgradeCostForBuildingBlock(ABuildingBlock* BuildingBlock)
{
	FInventoryItem UpgradeCost;
	switch (BuildingBlock->GetMaterialType())
	{
	case EToolType::WOOD:
		UpgradeCost.Name = GetResourceIDFromMaterialType(EToolType::STONE);
		break;
	case EToolType::STONE:
		UpgradeCost.Name = GetResourceIDFromMaterialType(EToolType::METAL);
		break;
	default:
		UpgradeCost.Name = GetResourceIDFromMaterialType(EToolType::WOOD);
		break;
	}
	UpgradeCost.Quantity = BuildingBlock->GetMaxDurability() * 0.75f;

	return UpgradeCost;
}

FInventoryItem ABuildingHammerItem::GetDestructionRefundForDeployable(ADeployable* Deployable)
{
	FInventoryItem RefundItem;
	RefundItem.Name = GetResourceIDFromMaterialType(Deployable->GetMaterialType());
	RefundItem.Quantity = FMath::Clamp(Deployable->GetCurrentDurability() * 0.25f, 1, 100);
	return RefundItem;
}

void ABuildingHammerItem::OnSwingImpact(const FHitResult& HitResult, const FVector& OwnerCharacterLookVector, bool FromFirstPersonInstance)
{
	Super::OnSwingImpact(HitResult, OwnerCharacterLookVector, FromFirstPersonInstance);
	
	if (!HasAuthority())
	{
		return;
	}

	ADeployable* HitDeployable = Cast<ADeployable>(HitResult.GetActor());
	if (HitDeployable == nullptr)
	{
		return;
	}
	
	AttemptDeployableRepair(HitDeployable, HitResult, OwnerCharacterLookVector);
}

void ABuildingHammerItem::ClearWidget()
{
	Widget = nullptr;
}

bool ABuildingHammerItem::HasAuthorization(const FVector& LocationToTest) const
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
	{
		return false;
	}

	UBuilderComponent* OwnerBuilderComponent = OwnerActor->FindComponentByClass<UBuilderComponent>();
	if (OwnerBuilderComponent == nullptr)
	{
		return false;
	}

	return OwnerBuilderComponent->HasAuthorizationAtLocation(LocationToTest);
}

void ABuildingHammerItem::AttemptDeployableRepair(ADeployable* DeployableToRepair, const FHitResult& HitResult, const FVector& DirectionVector)
{
	// Check if this deployable is damaged
	if (DeployableToRepair->GetCurrentDurability() >= DeployableToRepair->GetMaxDurability())
	{
		return;
	}

	FInventoryItem RepairCost;
	if (!CanRepairDeployable(DeployableToRepair, RepairCost))
	{
		return;
	}

	UInventoryComponent* OwnerInventoryComponent = GetOwner()->FindComponentByClass<UInventoryComponent>();
	if (OwnerInventoryComponent == nullptr)
	{
		return;
	}

	OwnerInventoryComponent->RemoveItem(RepairCost);

	const float RepairAmount = -(RepairCost.Quantity);
	FPointDamageEvent HitByToolEvent(RepairAmount, HitResult, DirectionVector, nullptr);
	DeployableToRepair->TakeDamage(RepairAmount, HitByToolEvent, GetOwnerPawn()->GetController(), this);
}

bool ABuildingHammerItem::CanRepairDeployable(ADeployable* DeployableToRepair, FInventoryItem& RepairCost) const
{
	if (DeployableToRepair == nullptr)
	{
		return false;
	}

	RepairCost.Name = GetResourceIDFromMaterialType(DeployableToRepair->GetMaterialType());
	RepairCost.Quantity = MaxRepairAmount;

	// Check if the player has this amount of resources?
	UInventoryComponent* OwnerInventoryComponent = GetOwner()->FindComponentByClass<UInventoryComponent>();
	if (OwnerInventoryComponent == nullptr || !OwnerInventoryComponent->GetContents()->HasItem(RepairCost.Name))
	{
		return false;
	}

	// if the deployable needs less than we are willing to give, we need to cut back and only use the amount required
	const float DurabilityDifference = DeployableToRepair->GetMaxDurability() - DeployableToRepair->GetCurrentDurability();
	if (DurabilityDifference < RepairCost.Quantity)
	{
		RepairCost.Quantity = DurabilityDifference;
	}

	// if the player is lacking sufficient resources we must reduce the amount of repair performed
	const int32 PlayerResourceQuantity = OwnerInventoryComponent->GetContents()->GetItemQuantity(RepairCost.Name);
	if (PlayerResourceQuantity < RepairCost.Quantity)
	{
		RepairCost.Quantity = PlayerResourceQuantity;
	}

	return true;
}

bool ABuildingHammerItem::LineTraceOnVisibility(FHitResult& OutHitResult) const
{
	const UEquipComponent* OwnerEquipComponent = GetOwnerEquipComponent();
	if (OwnerEquipComponent == nullptr)
	{
		return false;
	}
	
	const FVector OwnerCharacterLookVector = UKismetMathLibrary::GetForwardVector(OwnerEquipComponent->GetOwnerControlRotation());
	const FVector Start = GetOwnerPawn()->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
	const FVector End = Start + (OwnerCharacterLookVector * EffectiveRangeCentimeters);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bTraceComplex = true;
	CollisionParams.bReturnPhysicalMaterial = true;
	
	return GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams);
}
