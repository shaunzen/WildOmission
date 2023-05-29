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

UCLASS()
class WILDOMISSION_API AStorm : public AActor, public ISavableObjectInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AStorm();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Serialize(FArchive& Ar) override;

	void OnSpawn(const FVector2D& InWorldSize);

	void SetSeverity(float NewSeverity);

	UFUNCTION(BlueprintCallable)
	bool IsRaining(float& OutDensity) const;

	void SetLocalPlayerUnderneath(bool IsUnder);

	void HandleDestruction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetSpawnLocation(FVector& OutStartLocation);

	UPROPERTY(SaveGame)
	FVector2D WorldSize;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* StormRootComponent;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CloudMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* RainHazeComponent;

	UPROPERTY(SaveGame)
	FVector SpawnLocation;
	UPROPERTY(SaveGame)
	FVector TargetLocation;
	UPROPERTY(SaveGame)
	float DistanceToTravel;
	UPROPERTY(SaveGame)
	float DistanceTraveled;
	UPROPERTY(SaveGame)
	float MovementSpeed;
	UPROPERTY(SaveGame)
	FVector MovementVector;

	UPROPERTY(VisibleAnywhere, Replicated, SaveGame)
	float Severity;
	UPROPERTY(SaveGame)
	float SeverityMultiplier;

	UPROPERTY(EditDefaultsOnly)
	float RainSeverityThreshold;
	UPROPERTY(EditDefaultsOnly)
	float TornadoSeverityThreshold;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ALightning> LightningClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATornado> TornadoClass;

	UPROPERTY(SaveGame)
	bool HasSpawnedTornado;

	UPROPERTY(SaveGame)
	FTornadoSaveInformation TornadoSave;

	UPROPERTY(Replicated)
	ATornado* SpawnedTornado;

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
