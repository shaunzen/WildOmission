// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AISpawnHandler.generated.h"

class AWildOmissionAICharacter;
struct FAISpawnData;

UCLASS()
class WILDOMISSIONAI_API AAISpawnHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAISpawnHandler();
	void Setup(ATimeOfDayHandler* InTimeOfDayHandler);

	static TArray<AWildOmissionAICharacter*>* GetSpawnedCharacters();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	FTimerHandle NextSpawnCheckTimerHandle;

	UFUNCTION()
	void CheckSpawnConditionsForAllPlayers();
	void CheckSpawnConditionsForPlayer(APawn* PlayerToCheck);

	int32 GetNumAICharactersWithinRadiusFromLocation(const FVector& TestLocation) const;

	UFUNCTION()
	void SpawnAICharactersInRadiusFromOrigin(const FVector& SpawnOrigin);

	UFUNCTION()
	void RemoveAICharacterFromList(AWildOmissionAICharacter* MonsterToRemove);

	FTransform GetSpawnTransform(const FVector& SpawnOrigin) const;

	static FAISpawnData* GetSpawnData(const FName& AIName);

};
