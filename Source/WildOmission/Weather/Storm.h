// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Storm.generated.h"

class ATornado;
class UNiagaraComponent;

UCLASS()
class WILDOMISSION_API AStorm : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AStorm();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	FVector2D WorldSize;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* StormRootComponent;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CloudMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* RainHazeComponent;

	FVector SpawnLocation;
	FVector TargetLocation;
	float DistanceToTravel;
	float DistanceTraveled;

	float MovementSpeed;
	FVector MovementVector;

	UPROPERTY(VisibleAnywhere, Replicated, SaveGame)
	float Severity;
	float SeverityMultiplier;

	UPROPERTY(EditDefaultsOnly)
	float RainSeverityThreshold;
	
	UPROPERTY(EditDefaultsOnly)
	float TornadoSeverityThreshold;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATornado> TornadoClass;

	UPROPERTY(Replicated)
	ATornado* SpawnedTornado;

	bool LocalPlayerUnder;

	void HandleMovement();
	void HandleSeverity();
	void SpawnTornado();
};
