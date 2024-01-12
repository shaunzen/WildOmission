// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SavableObject.h"
#include "Interfaces/DurabilityInterface.h"
#include "Ragdoll.generated.h"

class UTimerDespawnComponent;
class UHarvestableComponent;

UCLASS()
class RAGDOLL_API ARagdoll : public AActor, public ISavableObject
{
	GENERATED_BODY()

public:
	ARagdoll();

	virtual void Tick(float DeltaTime) override;

	// Begin ISavableObject Implementation
	virtual FName GetIdentifier() const override;
	// End ISavableObject Implementation

	USkeletalMeshComponent* GetMeshComponent() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RagdollRootComponent;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	UTimerDespawnComponent* DespawnComponent;

	UPROPERTY(VisibleAnywhere)
	UHarvestableComponent* HarvestableComponent;

	UPROPERTY(EditDefaultsOnly)
	FName Identifier;

	void HandleInWater();
	void UpdateAttachedChunk();

};
