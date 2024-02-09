// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/FirearmItem.h"
#include "FullAutomaticFirearmItem.generated.h"

/**
 * 
 */
UCLASS()
class WEAPONS_API AFullAutomaticFirearmItem : public AFirearmItem
{
	GENERATED_BODY()
	
public:
	virtual void OnPrimaryHeld() override;

};
