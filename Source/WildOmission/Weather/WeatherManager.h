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

	AStorm* SpawnStorm(bool FromCommand = false);
	void ClearStorm();
	
	AStorm* GetCurrentStorm() const;
	void SetCurrentStorm(AStorm* NewCurrentStorm);
	
	float GetNextStormSpawnTime() const;
	void SetNextStormSpawnTime(float NewSpawnTime);

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
	AStorm* CurrentStorm;
	bool CanSpawnStorm() const;

	UPROPERTY()
	FWindParameters WindParameters;
	void CalculateWindParameters();

	UFUNCTION(NetMulticast, Unreliable)
	void Client_UpdateWindParameters(const FWindParameters& NewParameters);

};
