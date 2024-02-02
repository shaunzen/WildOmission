// Copyright Telephone Studios. All Rights Reserved.


#include "Components/InGameAchievementsComponent.h"
#include "AchievementsManager.h"
#include "Log.h"

// Sets default values for this component's properties
UInGameAchievementsComponent::UInGameAchievementsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UInGameAchievementsComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UInGameAchievementsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInGameAchievementsComponent::UnlockAchievement(const FString& AchievementID)
{
	// If owner is not local, fire client rpc which will call this
	APlayerController* OwnerPlayerController = Cast<APlayerController>(GetOwner());
	if (OwnerPlayerController == nullptr)
	{
		UE_LOG(LogAchievements, Warning, TEXT("UInGameAchievementsComponent::UnlockAchievement Failed to give achievement %s, OwnerPlayerController was nullptr."), *AchievementID);
		return;
	}
	
	if (OwnerPlayerController->HasAuthority() && !OwnerPlayerController->IsLocalPlayerController())
	{
		UE_LOG(LogAchievements, Display, TEXT("UInGameAchievementsComponent::UnlockAchievement a client has unlocked achievement %s, sending rpc."), *AchievementID);
		Client_UnlockAchievement(AchievementID);
		return;
	}

	// Only perform unlock if owner is local
	UAchievementsManager* AchievementsManager = UAchievementsManager::GetAchievementsManager();
	if (AchievementsManager == nullptr)
	{
		UE_LOG(LogAchievements, Display, TEXT("UInGameAchievementsComponent::UnlockAchievement Failed to give achievement %s, AchievementsManager was nullptr."), *AchievementID);
		return;
	}

	AchievementsManager->UnlockAchievement(AchievementID);
}

FAchievementStatsData UInGameAchievementsComponent::GetStatsData() const
{
	return StatsData;
}

void UInGameAchievementsComponent::SetStatsData(const FAchievementStatsData& InStatsData)
{
	StatsData = InStatsData;
}

void UInGameAchievementsComponent::Client_UnlockAchievement_Implementation(const FString& AchievementID)
{
	UE_LOG(LogAchievements, Display, TEXT("Client_UnlockAchievement Recieved rpc from server, attempting to give achievement %s."), *AchievementID);
	UnlockAchievement(AchievementID);
}