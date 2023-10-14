// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SavableObject.h"
#include "Interfaces/DurabilityInterface.h"
#include "WildOmissionAIRagdoll.generated.h"

class UTimerDespawnComponent;
class UHarvestableComponent;

UCLASS()
class WILDOMISSIONAI_API AWildOmissionAIRagdoll : public AActor, public ISavableObject
{
	GENERATED_BODY()

public:
	AWildOmissionAIRagdoll();
	
	// Begin ISavableObject Implementation
	virtual FName GetIdentifier() const override;
	// End ISavableObject Implementation

	USkeletalMeshComponent* GetMeshComponent() const;

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	UTimerDespawnComponent* DespawnComponent;

	UPROPERTY(VisibleAnywhere)
	UHarvestableComponent* HarvestableComponent;

	UPROPERTY(EditDefaultsOnly)
	FName Identifier;

};
