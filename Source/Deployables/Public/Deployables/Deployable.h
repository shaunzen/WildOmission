// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BuildAnchorComponent.h"
#include "Interfaces/SavableObject.h"
#include "Interfaces/RequiredForLoad.h"
#include "Interfaces/DurabilityInterface.h"
//#include "WildOmission/Core/Interfaces/DamagedByWind.h"
#include "Deployable.generated.h"

UCLASS()
class DEPLOYABLES_API ADeployable : public AActor, public IDurabilityInterface, public ISavableObject, public IRequiredForLoad //public IDamagedByWind
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeployable();
	
	virtual void OnSpawn();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//virtual void ApplyWindDamage(AActor* WindCauser, float DamageMultiplier = 1.0f) override;

	virtual float TakeDamage(float DamageAmount, const struct FDamageEvent& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UStaticMesh* GetMesh() const;
	FTransform GetMeshTransform() const;

	virtual float GetDurabilityPercentage() override;

	bool CanSpawnOnGround() const;
	bool CanSpawnOnFloor() const;
	bool CanSpawnOnWall() const;
	TEnumAsByte<EBuildAnchorType> CanSpawnOnBuildAnchor() const;
	bool FollowsSurfaceNormal() const;
	bool CanRotate() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* PlacementSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* DestructionSound;

	UPROPERTY(EditDefaultsOnly)
	float MaxDurability;

	UPROPERTY(Replicated, SaveGame)
	float CurrentDurability;

	UPROPERTY(EditDefaultsOnly, Category = "Placement Settings")
	bool bCanSpawnOnGround;
	UPROPERTY(EditDefaultsOnly, Category = "Placement Settings")
	bool bCanSpawnOnFloor;
	UPROPERTY(EditDefaultsOnly, Category = "Placement Settings")
	bool bCanSpawnOnWall;
	UPROPERTY(EditDefaultsOnly, Category = "Placement Settings")
	TEnumAsByte<EBuildAnchorType> CanSpawnOnAnchor;
	UPROPERTY(EditDefaultsOnly, Category = "Placement Settings")
	bool bFollowsSurfaceNormal;
	UPROPERTY(EditDefaultsOnly, Category = "Placement Settings")
	bool bCanRotate;

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayPlacementSound();

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayDestructionSound();
};
