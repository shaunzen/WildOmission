// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tornado.generated.h"

class AStorm;
class UWindSuckerComponent;

USTRUCT()
struct FTornadoSaveInformation
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, SaveGame)
	bool WasSpawned = false;

	UPROPERTY(VisibleAnywhere, SaveGame)
	FTransform Transform = FTransform();

	UPROPERTY(VisibleAnywhere, SaveGame)
	float RotationSpeed = 0.0f;

	UPROPERTY(VisibleAnywhere, SaveGame)
	float MovementSpeed = 0.0f;

	UPROPERTY(VisibleAnywhere, SaveGame)
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, SaveGame)
	float TotalLifetime = 0.0f;

	UPROPERTY(VisibleAnywhere, SaveGame)
	float RemainingLifetime = 0.0f;
};

UCLASS()
class WILDOMISSION_API ATornado : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATornado();

	virtual void Tick(float DeltaTime) override;

	void HandleSpawn(AStorm* OwnerStorm);

	FTornadoSaveInformation GetSaveInformation();

	void LoadSaveInformation(const FTornadoSaveInformation& InSave, AStorm* OwnerStorm);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SuctionAnchor;

	UPROPERTY(VisibleAnywhere)
	UWindSuckerComponent* CloseSuctionComponent1;
	UPROPERTY(VisibleAnywhere)
	UWindSuckerComponent* CloseSuctionComponent2;
	UPROPERTY(VisibleAnywhere)
	UWindSuckerComponent* CloseSuctionComponent3;
	UPROPERTY(VisibleAnywhere)
	UWindSuckerComponent* CloseSuctionComponent4;
	UPROPERTY(VisibleAnywhere)
	UWindSuckerComponent* FarSuctionComponent;

	UPROPERTY(VisibleAnywhere)
	float RotationSpeed;

	UPROPERTY(VisibleAnywhere)
	float MovementSpeed;

	UPROPERTY(VisibleAnywhere)
	FVector TargetLocation;

	UPROPERTY(VisibleAnywhere)
	float StormRadius;

	UPROPERTY(VisibleAnywhere)
	float RemainingLifetime;

	UPROPERTY(VisibleAnywhere)
	float TotalLifetime;

	UPROPERTY()
	AStorm* OwnerStorm;

	FVector GetRandomLocationInStorm();

	UFUNCTION()
	void OnActorOverlapVortex(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void HandleMovement();
	void HandleDamage();
	void HandleRotation();

	void GetStormRadius();

};
