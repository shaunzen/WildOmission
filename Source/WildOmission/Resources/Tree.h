// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HarvestableResource.h"
#include "WildOmission/Core/Interfaces/InteractsWithTornado.h"
#include "Tree.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSION_API ATree : public AHarvestableResource, public IInteractsWithTornado
{
	GENERATED_BODY()
public:
	ATree();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnContactWithTornado() override;

private:
	UPROPERTY(Replicated, SaveGame)
	bool bIsStump;

	UFUNCTION()
	virtual void OnLoadComplete_Implementation() override;

	UPROPERTY()
	UStaticMesh* StumpMesh;

	void UpdateMeshState();

};
