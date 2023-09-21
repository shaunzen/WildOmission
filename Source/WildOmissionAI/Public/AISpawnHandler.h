// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AISpawnHandler.generated.h"

class UDataTable;
class AWildOmissionAICharacter;
struct FAISpawnData;

UCLASS()
class WILDOMISSIONAI_API AAISpawnHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAISpawnHandler();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	int32 MinSpawnCheckTimeSeconds;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxSpawnCheckTimeSeconds;

	UPROPERTY(EditDefaultsOnly)
	float InnerSpawnRadiusCentimeters;

	UPROPERTY(EditDefaultsOnly)
	float OuterSpawnRadiusCentimeters;

	UPROPERTY(EditDefaultsOnly)
	float SpawnChance;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* AISpawnDataTable;

	TArray<AWildOmissionAICharacter*> SpawnedAICharacters;

	virtual bool IsSpawnConditionValid();

	FAISpawnData* GetSpawnData(const FName& AIName);

	virtual AWildOmissionAICharacter* HandleAISpawn(UClass* Class, const FTransform& SpawnTransform);

private:
	UPROPERTY()
	FTimerHandle NextSpawnCheckTimerHandle;

	UFUNCTION()
	void CheckSpawnConditionsForAllPlayers();
	void CheckSpawnConditionsForPlayer(APawn* PlayerToCheck);

	int32 GetNumAICharactersWithinRadiusFromLocation(const FVector& TestLocation) const;

	UFUNCTION()
	void SpawnAICharactersInRadiusFromLocation(const FVector& SpawnLocation);

	UFUNCTION()
	void RemoveAICharacterFromList(AWildOmissionAICharacter* CharacterToRemove);

	FTransform GetSpawnTransform(const FVector& SpawnLocation) const;

};
