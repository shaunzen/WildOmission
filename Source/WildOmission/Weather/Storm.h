// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WildOmission/Core/Interfaces/SavableObjectInterface.h"
#include "Tornado.h"
#include "Storm.generated.h"

class AWeatherManager;
class ALightning;
class UNiagaraComponent;

USTRUCT()
struct FStormState
{
	GENERATED_BODY()
	
	UPROPERTY()
	float Severity;

	UPROPERTY()
	ATornado* SpawnedTornado;
};

UCLASS()
class WILDOMISSION_API AStorm : public AActor, public ISavableObjectInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AStorm();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Serialize(FArchive& Ar) override;

	void HandleSpawn();
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetSpawnLocation(FVector& OutStartLocation);

	FVector2D WorldSize;

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
	
	UPROPERTY(SaveGame)
	FVector SpawnLocation;
	UPROPERTY(SaveGame)
	FVector MovementVector;
	UPROPERTY(SaveGame)
	float MovementSpeed;
	UPROPERTY(SaveGame)
	float SeverityMultiplier;
	UPROPERTY(SaveGame)
	float Severity;
	UPROPERTY(SaveGame)
	FTornadoSaveInformation TornadoSave;
	UPROPERTY(SaveGame)
	bool HasSpawnedTornado;

	FVector TargetLocation;
	float TravelDistance;
	float TraveledDistance;
	ATornado* SpawnedTornado;


	void CalculateTargetLocation();
	void CalculateTravelDistance();
	void CalculateTraveledDistance();

	UPROPERTY()
	AWeatherManager* WeatherManager;

	bool LocalPlayerUnder;
	float NextLightningStrikeTime;

	void HandleMovement();
	void HandleSeverity();
	void HandleLightning();
	void SpawnLightning();
	void SpawnTornado(bool bFromSave = false);

	UFUNCTION()
	virtual void OnLoadComplete_Implementation() override;

};
