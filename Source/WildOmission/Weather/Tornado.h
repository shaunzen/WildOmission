// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tornado.generated.h"

class URadialForceComponent;

//TODO make savable
UCLASS()
class WILDOMISSION_API ATornado : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATornado();

	virtual void Tick(float DeltaTime) override;

	void OnSpawn(FVector2D InWorldSize);

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

	UPROPERTY(EditDefaultsOnly)
	float RotationSpeed;

	UPROPERTY(EditDefaultsOnly)
	float MovementSpeed;

	UPROPERTY(VisibleAnywhere)
	FVector TargetLocation;
	
	UPROPERTY(VisibleAnywhere)
	FVector OldTargetLocation;

	FVector2D WorldSize;

	void GetNewTargetLocation();
	FVector GetRandomLocationInWorld();

	void HandleMovement();
	void HandleDamage();
	void HandleRotation();

};
