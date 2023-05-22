// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Storm.generated.h"

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

	void OnSpawn(const FVector2D& InWorldSize);

	void SetSeverity(float NewSeverity);

	UFUNCTION(BlueprintCallable)
	bool IsRaining(float& OutDensity) const;

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

	FVector SpawnLocation;
	FVector TargetLocation;
	float DistanceToTravel;
	float DistanceTraveled;

	float MovementSpeed;
	FVector MovementVector;

	UPROPERTY(EditAnywhere)
	float Severity;
	float SeverityMultiplier;

	UPROPERTY(EditDefaultsOnly)
	float RainSeverityThreshold;
	
	UPROPERTY(EditDefaultsOnly)
	float TornadoSeverityThreshold;

	void HandleMovement();
	void HandleSeverity();

};
