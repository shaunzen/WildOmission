// Copyright Telephone Studios. All Rights Reserved.


#include "WOInGameAchievementsComponent.h"
#include "WildOmissionCore/Characters/WildOmissionCharacter.h"

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
	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
	{
		return;
	}

	// Player Death is created, just bind
	if (AWildOmissionCharacter* OwnerCharacter = Cast<AWildOmissionCharacter>(OwnerActor))
	{
		OwnerCharacter->OnPlayerDeath.AddDynamic(this, &UWOInGameAchievementsComponent::GiveDeathAchievement);
	}

}

void UWOInGameAchievementsComponent::GiveDeathAchievement()
{
	this->UnlockAchievement(ACH_RIP);
}
