// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeatherManager.generated.h"

class AStorm;

USTRUCT()
struct FWindParameters
{
	GENERATED_BODY()
	
	UPROPERTY()
	float GlobalWindStrength = 0.3f;

	UPROPERTY()
	FVector2D GlobalWindDirection = FVector2D(1.0f, 0.0f);

	UPROPERTY()
	float TornadoOnGround = 0.0f;

	UPROPERTY()
	FVector2D TornadoLocation = FVector2D::ZeroVector;

};

UCLASS()
class WILDOMISSION_API AWeatherManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeatherManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetNextStormChanceTime() const;
	void SetNextStormChanceTime(float NewTime);

	// Returns the storm that was spawned
	AStorm* SpawnStorm();

	void ClearAllStorms();

	void RemoveStormFromList(AStorm* StormToRemove);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditDefaultsOnly)
	float MinTimeBetweenStorms;
	UPROPERTY(EditDefaultsOnly)
	float MaxTimeBetweenStorms;

	UPROPERTY(EditAnywhere)
	float NextStormChanceTime;

	UPROPERTY()
	TSubclassOf<AStorm> StormClass;
	
	UPROPERTY()
	TArray<AStorm*> SpawnedStorms;

	UPROPERTY()
	FWindParameters WindParameters;

	void CalculateWindParameters();

	void GetNewStormChanceTime();
	void TrySpawnStorm();

	UFUNCTION(NetMulticast, Unreliable)
	void Client_UpdateWindParameters(const FWindParameters& NewParameters);

};
