// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/SwingableItem.h"
#include "MonsterArmsItem.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERS_API AMonsterArmsItem : public ASwingableItem
{
	GENERATED_BODY()
	
public:
	AMonsterArmsItem();
	
	virtual void OnPrimaryHeld() override;

};
