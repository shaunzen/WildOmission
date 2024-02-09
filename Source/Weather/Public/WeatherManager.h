// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeatherManager.generated.h"

UCLASS()
class WEATHER_API AWeatherManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeatherManager();
	
	static void SetWeatherManager(AWeatherManager* NewInstance);
	static AWeatherManager* GetWeatherManager();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Save(struct FWeatherData& OutWeatherData);
	void Load(const struct FWeatherData& InWeatherData);

	class AStorm* SpawnStorm(bool FromCommand = false);
	void ClearStorm();
	void SetCurrentStorm(class AStorm* NewCurrentStorm);
	void SetStormsDisabled(bool InStormsDisabled);
	
	class AStorm* GetCurrentStorm() const;
	bool CanSpawnStorm() const;
	bool GetStormsDisabled() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(VisibleAnywhere)
	class UWeatherSaveComponent* SaveComponent;

	UPROPERTY(EditDefaultsOnly)
	TArray<float> StormSpawnChances;
	
	UFUNCTION()
	void AttemptToSpawnStorm();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AStorm> StormClass;
	
	bool StormsDisabled;

	UPROPERTY(Replicated)
	class AStorm* CurrentStorm;

	UFUNCTION()
	void OnStormCleanup();

	void UpdateWindParameters();
	void ApplyWindParameters(const struct FWindParameters& NewParameters);

};
