// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

UCLASS()
class WEATHER_API AWeatherHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeatherHandler();
	
	static AWeatherHandler* GetWeatherHandler();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:	
	UPROPERTY(EditAnywhere)
	float NextStormSpawnTime;

	UPROPERTY(EditDefaultsOnly)
	float MinStormSpawnTime;
	UPROPERTY(EditDefaultsOnly)
	float MaxStormSpawnTime;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AStorm> StormClass;
	
	UPROPERTY(Replicated)
	AStorm* CurrentStorm;
	bool CanSpawnStorm() const;

	void CalculateWindParameters();
	void ApplyWindParameters(const FWindParameters& NewParameters);

};
