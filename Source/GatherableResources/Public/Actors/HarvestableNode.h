// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/HarvestableResource.h"
#include "HarvestableNode.generated.h"

/**
 * 
 */
UCLASS()
class GATHERABLERESOURCES_API AHarvestableNode : public AHarvestableResource
{
	GENERATED_BODY()
	
public:
	AHarvestableNode();
	

protected:
	virtual void OnRep_NormalizedDurability() override;

	virtual void RefreshNodeStage();
	
private:
	UPROPERTY(EditDefaultsOnly)
	TArray<UStaticMesh*> MeshStages;
	
};
