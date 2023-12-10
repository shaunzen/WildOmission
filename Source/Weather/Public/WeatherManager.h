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
class WEATHER_API AWeatherManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeatherManager();
	
	static AWeatherManager* GetWeatherManager();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsPeacefulMode() const;

	AStorm* SpawnStorm(bool FromCommand = false);
	void ClearStorm();
	
	AStorm* GetCurrentStorm() const;
	void SetCurrentStorm(AStorm* NewCurrentStorm);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:	
	UPROPERTY(EditDefaultsOnly)
	float SunriseStormSpawnChance;
	UPROPERTY(EditDefaultsOnly)
	float NoonStormSpawnChance;
	UPROPERTY(EditDefaultsOnly)
	float SunsetStormSpawnChance;
	UPROPERTY(EditDefaultsOnly)
	float MidnightStormSpawnChance;

	UFUNCTION()
	void AttemptSunriseStorm();

	UFUNCTION()
	void AttemptNoonStorm();

	UFUNCTION()
	void AttemptSunsetStorm();

	UFUNCTION()
	void AttemptMidnightStorm();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AStorm> StormClass;
	
	UPROPERTY(Replicated)
	AStorm* CurrentStorm;
	bool CanSpawnStorm() const;

	void CalculateWindParameters();
	void ApplyWindParameters(const FWindParameters& NewParameters);

};
