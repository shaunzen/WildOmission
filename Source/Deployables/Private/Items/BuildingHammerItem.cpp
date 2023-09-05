// Copyright Telephone Studios. All Rights Reserved.


#include "Items/BuildingHammerItem.h"
#include "Components/EquipComponent.h"
#include "Interfaces/DurabilityInterface.h"
#include "Components/InventoryComponent.h"
#include "UI/BuildingHammerWidget.h"
#include "Deployables/Deployable.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DamageEvents.h"
#include "UObject/ConstructorHelpers.h"

static TSubclassOf<UBuildingHammerWidget> WidgetClass = nullptr;

ABuildingHammerItem::ABuildingHammerItem()
{
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
	if (HitDeployable == nullptr)
	{
		return;
	}

	if (Widget)
	{
		Widget->Teardown();
	}

	// TODO show menu
	Widget = CreateWidget<UBuildingHammerWidget>(OwnerPlayerController, WidgetClass);
	if (Widget == nullptr)
	{
		return;
	}
	Widget->OnTeardown.AddDynamic(this, &ABuildingHammerItem::ClearWidget);
	Widget->Setup(HitDeployable);
}

void ABuildingHammerItem::OnPrimaryAnimationClimax(bool FromFirstPersonInstance)
{
	FHitResult HitResult;
	if (!LineTraceOnVisibility(HitResult) || GetOwnerEquipComponent() == nullptr)
	{
		return;
	}

	const FVector OwnerCharacterLookVector = UKismetMathLibrary::GetForwardVector(GetOwnerEquipComponent()->GetOwnerControlRotation());

	if (FromFirstPersonInstance || !GetOwnerPawn()->IsLocallyControlled())
	{
		PlayImpactSound(HitResult);
		SpawnImpactParticles(HitResult, OwnerCharacterLookVector);
		//SpawnImpactDecal(HitResult);
	}

	if (!HasAuthority())
	{
		return;
	}
	
	ADeployable* HitDeployable = Cast<ADeployable>(HitResult.GetActor());
	if (HitDeployable)
	{
		AttemptDeployableRepair(HitDeployable, HitResult, OwnerCharacterLookVector);
		UpdateDurability();
	}

}

void ABuildingHammerItem::OnUnequip()
{
	Super::OnUnequip();

	if (Widget)
	{
		Widget->Teardown();
	}
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

void ABuildingHammerItem::ClearWidget()
{
	Widget = nullptr;
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

	switch (DeployableToRepair->GetMaterialType())
	{
	case EToolType::WOOD:
		RepairCost.Name = TEXT("wood");
		break;
	case EToolType::STONE:
		RepairCost.Name = TEXT("stone");
		break;
	case EToolType::METAL:
		RepairCost.Name = TEXT("metal");
		break;
	default:
		RepairCost.Name = TEXT("wood");
		break;
	}

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
