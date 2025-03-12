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

	void Setup(bool SpawnedFromCommand = false);

	void SetBoundsRadius(float InBoundsRadius);
	float GetBoundsRadius() const;

	FTornadoData GetTornadoData() const;

	void LoadTornadoData(const FTornadoData& InTornadoData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleAnywhere)
	class UChunkInvokerComponent* ChunkInvokerComponent;

	//*********************************************************
	//	Suction Components
	//*********************************************************
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

	//*********************************************************
	//	Movement Parameters
	//*********************************************************
	UPROPERTY(VisibleAnywhere)
	float RotationSpeed;
	UPROPERTY(VisibleAnywhere)
	float MovementSpeed;
	UPROPERTY(VisibleAnywhere)
	FVector TargetLocation;
	UPROPERTY(VisibleAnywhere)
	float BoundsRadius;
	UPROPERTY(VisibleAnywhere)
	float RemainingLifetime;
	UPROPERTY(VisibleAnywhere)
	float TotalLifetime;
	UPROPERTY(VisibleAnywhere)
	float Severity;

	FVector GetRandomLocationInBounds() const;

	void OnUpdateSeverity();

	UFUNCTION()
	void UpdateMovement();
	UFUNCTION()
	void UpdateDamage();
	UFUNCTION()
	void UpdateRotation();

	bool HasLineOfSightTo(AActor* InActor) const;

	UFUNCTION()
	void OnActorOverlapVortex(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
