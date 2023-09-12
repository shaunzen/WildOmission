// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/WeaponProjectile.h"
#include "Interfaces/Interactable.h"
#include "ArrowProjectile.generated.h"

/**
 * 
 */
UCLASS()
class WEAPONS_API AArrowProjectile : public AWeaponProjectile, public IInteractable
{
	GENERATED_BODY()
	
public:
	AArrowProjectile();

	// Begin IInteractable Implementation
	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;
	// End IInteractable Implementation

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	FName ItemID;

};
