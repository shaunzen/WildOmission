// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Storm.generated.h"

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

	float Severity;
	float SeverityMultiplier;

};
