// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/DeployableItemBase.h"
#include "LockApplicationItem.generated.h"

/**
 * 
 */
UCLASS()
class ALockApplicationItem : public ADeployableItemBase
{
	GENERATED_BODY()

public:
	ALockApplicationItem();

protected:
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* LockMesh;

	virtual void OnPlace() override;
	virtual bool GetPlacementTransform(FTransform& OutPlacementTransform);
	virtual UStaticMesh* GetPreviewMesh();

};
