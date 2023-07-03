// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SavableWeatherHandler.h"
#include "WeatherHandler.generated.h"

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

class AWorldGenerationHandler;

UCLASS()
class WEATHER_API AWeatherHandler : public AActor, public ISavableWeatherHandler
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeatherHandler();
	
	void Setup(AWorldGenerationHandler* InWorldGenerationHandler);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AStorm* SpawnStorm(bool FromCommand = false);
	void ClearStorm();
	
	AStorm* GetCurrentStorm() const;
	void SetCurrentStorm(AStorm* NewCurrentStorm);
	
	// Begin ISavableWeatherHandler Implementation
	virtual float GetNextStormSpawnTime() const override;
	virtual void SetNextStormSpawnTime(float NewSpawnTime) override;
	// End ISavableWeatherHandler Implementation

	AWorldGenerationHandler* GetWorldGenerationHandler() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere)
	float NextStormSpawnTime;

	UPROPERTY(EditDefaultsOnly)
	float MinStormSpawnTime;
	UPROPERTY(EditDefaultsOnly)
	float MaxStormSpawnTime;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AStorm> StormClass;
	
	UPROPERTY()
	AWorldGenerationHandler* WorldGenerationHandler;

	UPROPERTY()
	AStorm* CurrentStorm;
	bool CanSpawnStorm() const;

	UPROPERTY()
	FWindParameters WindParameters;
	void CalculateWindParameters();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_UpdateWindParameters(const FWindParameters& NewParameters);

};
