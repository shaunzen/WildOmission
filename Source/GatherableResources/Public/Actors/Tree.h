// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HarvestableResource.h"
#include "Interfaces/InteractsWithTornado.h"
#include "Tree.generated.h"

/**
 * 
 */
UCLASS()
class GATHERABLERESOURCES_API ATree : public AHarvestableResource, public IInteractsWithTornado
{
	GENERATED_BODY()
public:
	ATree();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnContactWithTornado() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* DustBoundsComponent;

	UPROPERTY(Replicated, ReplicatedUsing = UpdateMeshState, SaveGame)
	bool bIsStump;

	UFUNCTION()
	virtual void OnLoadComplete_Implementation() override;

	UPROPERTY()
	UStaticMesh* StumpMesh;

	UFUNCTION()
	void UpdateMeshState();

	virtual void PlayDestructionEffects() override;

};
