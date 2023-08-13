// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FirearmProjectile.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;

UCLASS()
class WEAPONS_API AFirearmProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFirearmProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* TrailComponent;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;
	
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly)
	float Damage;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void SpawnImpactEffects(const FHitResult& HitResult);

};
