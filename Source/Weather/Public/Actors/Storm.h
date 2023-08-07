// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SavableObject.h"
#include "Tornado.h"
#include "Storm.generated.h"

class AWeatherHandler;
class ALightning;
class UNiagaraComponent;

UCLASS()
class WEATHER_API AStorm : public AActor, public ISavableObject
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AStorm();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Serialize(FArchive& Ar) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void HandleSpawn(bool SpawnedFromCommand = false);
	void HandleDestruction();

	float GetSeverity() const;
	void SetSeverity(float NewSeverity);

	float GetTravelDistance() const;
	float GetTraveledDistance() const;
	FVector GetMovementVector() const;
	ATornado* GetSpawnedTornado() const;

	UFUNCTION(BlueprintCallable)
	bool IsRaining(float& OutDensity) const;
	void SetLocalPlayerUnderneath(bool IsUnder);

	// Begin ISavableObject Implementation
	virtual FName GetIdentifier() const override;
	// End ISavableObject Implementation

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* StormRootComponent;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CloudMeshComponent;
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* RainHazeComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ALightning> LightningClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATornado> TornadoClass;

	UPROPERTY(EditDefaultsOnly)
	float RainSeverityThreshold;
	UPROPERTY(EditDefaultsOnly)
	float TornadoSeverityThreshold;
	
	UPROPERTY(EditDefaultsOnly, Category = "Save System")
	FName Identifier;

	UPROPERTY(SaveGame)
	FVector SpawnLocation;
	UPROPERTY(Replicated, SaveGame)
	FVector MovementVector;
	UPROPERTY(SaveGame)
	float MovementSpeed;
	UPROPERTY(SaveGame)
	float SeverityMultiplier;
	UPROPERTY(Replicated, SaveGame)
	float Severity;
	UPROPERTY(SaveGame)
	FTornadoSaveInformation TornadoSave;
	UPROPERTY(SaveGame)
	bool HasSpawnedTornado;

	FVector TargetLocation;
	float TravelDistance;
	float TraveledDistance;
	UPROPERTY(Replicated)
	ATornado* SpawnedTornado;

	UPROPERTY()
	AWeatherHandler* WeatherHandler;

	bool WasSpawnedFromCommand;

	bool LocalPlayerUnder;
	float NextLightningStrikeTime;

	void GetSpawnLocation();
	void CalculateTargetLocation();
	void CalculateTravelDistance();
	void CalculateTraveledDistance();

	// Client-Side Logic
	void HandleCloudAppearance();

	// Server-Side Logic
	void HandleMovement();
	void HandleSeverity();
	void HandleLightning();
	void SpawnLightning();
	void SpawnTornado(bool bFromSave = false);

	UFUNCTION()
	virtual void OnLoadComplete_Implementation() override;

};
