// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/TornadoData.h"
#include "Storm.generated.h"

UCLASS()
class WEATHER_API AStorm : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AStorm();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Serialize(FArchive& Ar) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void OnLoadComplete();

	void HandleSpawn(bool SpawnedFromCommand = false);
	void HandleDestruction();

	float GetSeverity() const;
	void SetSeverity(float NewSeverity);

	FVector GetStormTargetLocation() const;
	float GetTravelDistance() const;
	float GetDistanceTraveled() const;

	void SetMovementVector(const FVector& InMovementVector);

	FVector GetMovementVector() const;
	class ATornado* GetSpawnedTornado() const;

	UFUNCTION(BlueprintCallable)
	bool IsRaining(float& OutDensity) const;
	void SetLocalPlayerUnderneath(bool IsUnder);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* StormRootComponent;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CloudMeshComponent;
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* RainHazeComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ALightning> LightningClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ATornado> TornadoClass;

	UPROPERTY(EditDefaultsOnly)
	float RainSeverityThreshold;
	UPROPERTY(EditDefaultsOnly)
	float TornadoSeverityThreshold;

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
	FTornadoData TornadoData;
	UPROPERTY(SaveGame)
	bool HasSpawnedTornado;

	UPROPERTY(Replicated)
	class ATornado* SpawnedTornado;

	bool WasSpawnedFromCommand;

	bool LocalPlayerUnder;
	float NextLightningStrikeTime;

	void GetSpawnData(FVector& OutSpawnLocation, FVector& OutMovementVector) const;

	// Client-Side Logic
	void HandleCloudAppearance();

	// Server-Side Logic
	void HandleMovement();
	void HandleSeverity();
	void HandleLightning();
	void SpawnLightning();
	void SpawnTornado(bool bFromSave = false);

};
