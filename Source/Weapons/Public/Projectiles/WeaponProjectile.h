// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponProjectile.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;
class ACollectableProjectile;

UCLASS()
class WEAPONS_API AWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* TrailComponent;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<ACollectableProjectile> CollectableClass;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY()
	USoundBase* HitMarkerSound;
	
	UPROPERTY()
	USoundBase* HitMarkerHeadshotSound;

	UFUNCTION(NetMulticast, Reliable)
	void Multi_SpawnImpactEffects(const FHitResult& HitResult);

	UFUNCTION(Client, Reliable)
	void Client_PlayHitMarkerSound();

	UFUNCTION(Client, Reliable)
	void Client_PlayHitMarkerHeadshotSound();

};
