// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/FirearmItem.h"
#include "PookaCannon.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERS_API APookaCannon : public AFirearmItem
{
	GENERATED_BODY()

public:
	virtual void OnPrimaryHeld() override;

};
