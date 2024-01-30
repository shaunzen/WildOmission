// Copyright Telephone Studios. All Rights Reserved.


#include "WOInGameAchievementsComponent.h"
#include "WildOmissionCore/Characters/WildOmissionCharacter.h"

// Wild Omission Components
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

void UWOInGameAchievementsComponent::BeginPlay()
{
	// TODO setup delegates to call achievements
	APlayerController* OwnerPlayerController = Cast<APlayerController>(GetOwner());
	if (OwnerPlayerController == nullptr)
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

	// Character specific achievements
	OwnerCharacter->OnPlayerDeath.AddDynamic(this, &UWOInGameAchievementsComponent::GiveDeathAchievement);

	UCraftingComponent* OwnerCraftingComponent = OwnerCharacter->GetCraftingComponent();
	if (OwnerCraftingComponent)
	{
		OwnerCraftingComponent->OnItemCrafted.AddDynamic(this, &UWOInGameAchievementsComponent::CheckCraftAchievementConditions);
	}
}

void UWOInGameAchievementsComponent::GiveDeathAchievement()
{
	this->UnlockAchievement(ACH_RIP);
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
