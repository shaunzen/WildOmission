// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/TornadoData.h"
#include "Tornado.generated.h"

UCLASS()
class WEATHER_API ATornado : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATornado();

	virtual void Tick(float DeltaTime) override;

	void HandleSpawn(class AStorm* OwnerStorm, bool SpawnedFromCommand = false);

	FTornadoData GetTornadoData();

	void LoadTornadoData(const FTornadoData& InTornadoData, class AStorm* OwnerStorm);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleAnywhere)
	class UChunkInvokerComponent* ChunkInvokerComponent;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SuctionAnchor;

	UPROPERTY(VisibleAnywhere)
	class UWindSuckerComponent* CloseSuctionComponent1;
	UPROPERTY(VisibleAnywhere)
	class UWindSuckerComponent* CloseSuctionComponent2;
	UPROPERTY(VisibleAnywhere)
	class UWindSuckerComponent* CloseSuctionComponent3;
	UPROPERTY(VisibleAnywhere)
	class UWindSuckerComponent* CloseSuctionComponent4;
	UPROPERTY(VisibleAnywhere)
	class UWindSuckerComponent* FarSuctionComponent;

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
	class AStorm* OwnerStorm;

	FVector GetRandomLocationInStorm();

	UFUNCTION()
	void OnActorOverlapVortex(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleMovement();
	UFUNCTION()
	void HandleDamage();
	UFUNCTION()
	void HandleRotation();

	void GetStormRadius();

	bool HasLineOfSightTo(AActor* InActor) const;

};
