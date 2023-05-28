// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tornado.generated.h"

class URadialForceComponent;

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
	FVector OldTargetLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, SaveGame)
	float StormRadius = 0.0f;

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

	void OnSpawn(const float& InStormRadius);

	FTornadoSaveInformation GetSaveInformation();

	void LoadSaveInformation(const FTornadoSaveInformation& InSave);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RadialSuctionAnchor;

	UPROPERTY(VisibleAnywhere)
		URadialForceComponent* RadialSuctionComponent1;
	UPROPERTY(VisibleAnywhere)
		URadialForceComponent* RadialSuctionComponent2;
	UPROPERTY(VisibleAnywhere)
		URadialForceComponent* RadialSuctionComponent3;
	UPROPERTY(VisibleAnywhere)
		URadialForceComponent* RadialSuctionComponent4;

	UPROPERTY(VisibleAnywhere)
		URadialForceComponent* DistanceSuctionComponent;

	UPROPERTY(VisibleAnywhere)
	float RotationSpeed;

	UPROPERTY(VisibleAnywhere)
	float MovementSpeed;

	UPROPERTY(VisibleAnywhere)
	FVector TargetLocation;
	
	UPROPERTY(VisibleAnywhere)
	FVector OldTargetLocation;

	UPROPERTY(VisibleAnywhere)
	float StormRadius;

	UPROPERTY(VisibleAnywhere)
	float RemainingLifetime;

	UPROPERTY(VisibleAnywhere)
	float TotalLifetime;

	void GetNewTargetLocation();
	FVector GetRandomLocationInStorm();

	UFUNCTION()
	void OnActorOverlapVortex(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void HandleMovement();
	void HandleDamage();
	void HandleRotation();

};
