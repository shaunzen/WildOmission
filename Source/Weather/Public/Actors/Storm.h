// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/TornadoData.h"
#include "Storm.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStormCleanupSignature);

UCLASS()
class WEATHER_API AStorm : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AStorm();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Serialize(FArchive& Ar) override;
	void OnLoadComplete();

	void Setup(bool SpawnedFromCommand = false);
	void Cleanup();

	FOnStormCleanupSignature OnCleanup;

	void SetSeverity(float NewSeverity);
	void SetMovementVector(const FVector& InMovementVector);

	void SetLocalPlayerUnderneath(bool IsUnder);

	UFUNCTION(BlueprintCallable)
	bool IsRaining(float& OutDensity) const;
	float GetSeverity() const;
	FVector GetMovementVector() const;
	FVector GetStormTargetLocation() const;
	float GetTravelDistance() const;
	float GetDistanceTraveled() const;
	class ATornado* GetSpawnedTornado() const;

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
	void UpdateCloudAppearance();

	// Server-Side Logic
	void UpdateLocation();
	void UpdateSeverity();
	void SpawnTornado(bool bFromSave = false);
	

	// TODO lightning should be a multicast type of thing
	void HandleLightning();
	void SpawnLightning();

};
