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
	float RotationSpeed;

	UPROPERTY(VisibleAnywhere, SaveGame)
	float MovementSpeed;

	UPROPERTY(VisibleAnywhere, SaveGame)
	FVector TargetLocation;

	UPROPERTY(VisibleAnywhere, SaveGame)
	FVector OldTargetLocation;

	UPROPERTY(VisibleAnywhere, SaveGame)
	float StormRadius;

	UPROPERTY(VisibleAnywhere, SaveGame)
	float RemainingLifetime;
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

	void GetNewTargetLocation();
	FVector GetRandomLocationInStorm();

	UFUNCTION()
	void OnActorOverlapVortex(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void HandleMovement();
	void HandleDamage();
	void HandleRotation();

};
