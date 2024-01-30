// Copyright Telephone Studios. All Rights Reserved.


#include "WOInGameAchievementsComponent.h"
#include "WildOmissionCore/Characters/WildOmissionCharacter.h"

// Wild Omission Components
#include "Components/PlayerInventoryComponent.h"
#include "Components/CraftingComponent.h"

// List of defined in-game achievements
const static FString ACH_HEY_IM_UP_HERE(TEXT("ACH_HEY_IM_UP_HERE"));
const static FString ACH_UPGRADED_TOOL(TEXT("ACH_UPGRADED_TOOL"));
const static FString ACH_KISS_THE_CHEF(TEXT("ACH_KISS_THE_CHEF"));
const static FString ACH_BETTER_THAN_A_ROCK(TEXT("ACH_BETTER_THAN_A_ROCK"));
const static FString ACH_SLAYER(TEXT("ACH_SLAYER"));
const static FString ACH_IM_THE_REAL_LIFE(TEXT("ACH_IM_THE_REAL_LIFE"));
const static FString ACH_BITE_THE_HAND(TEXT("ACH_BITE_THE_HAND"));
const static FString ACH_ITS_GLOWING_BLUE(TEXT("ACH_ITS_GLOWING_BLUE"));
const static FString ACH_SHINY(TEXT("ACH_SHINY"));
const static FString ACH_THE_BLACKSMITH(TEXT("ACH_THE_BLACKSMITH"));
const static FString ACH_THE_MARKSMAN(TEXT("ACH_THE_MARKSMAN"));
const static FString ACH_RIP(TEXT("ACH_RIP"));
const static FString ACH_WOOD_COLLECTOR(TEXT("ACH_WOOD_COLLECTOR"));
const static FString ACH_STONE_COLLECTOR(TEXT("ACH_STONE_COLLECTOR"));
const static FString ACH_IM_A_MURDERER(TEXT("ACH_IM_A_MURDERER"));
const static FString ACH_BUILDER(TEXT("ACH_BUILDER"));

UWOInGameAchievementsComponent::UWOInGameAchievementsComponent()
{
}

void UWOInGameAchievementsComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* OwnerPlayerController = Cast<APlayerController>(GetOwner());
	if (OwnerPlayerController == nullptr || !OwnerPlayerController->HasAuthority())
	{
		return;
	}

	// Non character specific delegates
	OwnerPlayerController->OnPossessedPawnChanged.AddDynamic(this, &UWOInGameAchievementsComponent::OnOwnerPossessedPawnChanged);
}

void UWOInGameAchievementsComponent::OnOwnerPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	AWildOmissionCharacter* OwnerCharacter = Cast<AWildOmissionCharacter>(NewPawn);
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	// Well this is perhaps the most bizzar thing that has ever happened in my programming career thus far
	// Character Components were constantly returning nullptrs, I didn't change anything, and then all the sudden
	// it's working as intended. I read something about a solar storm this morning(1/30/24), as of right now that's my only guess.

	// Character specific delegates
	OwnerCharacter->OnPlayerDeath.AddDynamic(this, &UWOInGameAchievementsComponent::GiveDeathAchievement);

	UPlayerInventoryComponent* OwnerInventoryComponent = OwnerCharacter->FindComponentByClass<UPlayerInventoryComponent>();
	if (OwnerInventoryComponent)
	{
		OwnerInventoryComponent->OnItemHarvested.AddDynamic(this, &UWOInGameAchievementsComponent::OnItemHarvested);
	}

	UCraftingComponent* OwnerCraftingComponent = OwnerCharacter->FindComponentByClass<UCraftingComponent>();
	if (OwnerCraftingComponent)
	{
		OwnerCraftingComponent->OnItemCrafted.AddDynamic(this, &UWOInGameAchievementsComponent::CheckCraftAchievementConditions);
	}
}

void UWOInGameAchievementsComponent::GiveDeathAchievement()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, TEXT("Player Death occured."));
	this->UnlockAchievement(ACH_RIP);
}

void UWOInGameAchievementsComponent::OnItemHarvested(const FInventoryItem& ItemHarvested)
{
	// Wood Achievements
	if (ItemHarvested.Name == TEXT("wood"))
	{
		StatsData.WoodHarvested += ItemHarvested.Quantity;
		if (StatsData.WoodHarvested >= 1000)
		{
			this->UnlockAchievement(ACH_WOOD_COLLECTOR);
		}
	}
	// Stone Achievements
	else if (ItemHarvested.Name == TEXT("stone"))
	{
		StatsData.StoneHarvested += ItemHarvested.Quantity;
		if (StatsData.StoneHarvested >= 1000)
		{
			this->UnlockAchievement(ACH_STONE_COLLECTOR);
		}
	}
	// Metal Ore Achievements
	else if (ItemHarvested.Name == TEXT("metal.ore"))
	{
		StatsData.MetalOreHarvested += ItemHarvested.Quantity;
	}
	// Sulfur Ore Achievements
	else if (ItemHarvested.Name == TEXT("sulfur.ore"))
	{
		StatsData.SulfurOreHarvested += ItemHarvested.Quantity;
	}
	// Refined Metal Achievements
	else if (ItemHarvested.Name == TEXT("metal.refined"))
	{
		StatsData.RefinedMetalHarvested += ItemHarvested.Quantity;
		if (StatsData.RefinedMetalHarvested >= 10)
		{
			this->UnlockAchievement(ACH_ITS_GLOWING_BLUE);
		}
	}
	// Gold Achievements
	else if (ItemHarvested.Name == TEXT("gold"))
	{
		StatsData.GoldHarvested += ItemHarvested.Quantity;
		if (StatsData.GoldHarvested >= 10)
		{
			this->UnlockAchievement(ACH_SHINY);
		}
	}
}

void UWOInGameAchievementsComponent::CheckCraftAchievementConditions(const FName& ItemID)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, FString::Printf(TEXT("crafted %s"), *ItemID.ToString()));

	if (ItemID == TEXT("pickaxe") || ItemID == TEXT("hatchet"))
	{
		this->UnlockAchievement(ACH_UPGRADED_TOOL);
	}
	else if (ItemID == TEXT("pickaxe.stone") || ItemID == TEXT("hatchet.stone"))
	{
		this->UnlockAchievement(ACH_BETTER_THAN_A_ROCK);
	}
}
