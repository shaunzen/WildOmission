// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/FirearmItem.h"
#include "SemiAutomaticFirearmItem.generated.h"

/**
 * 
 */
UCLASS()
class WEAPONS_API ASemiAutomaticFirearmItem : public AFirearmItem
{
	GENERATED_BODY()

public:
	virtual void OnPrimaryPressed() override;
	
};
