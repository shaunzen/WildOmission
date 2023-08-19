// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/ToolItem.h"
#include "MonsterArmsItem.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERS_API AMonsterArmsItem : public AToolItem
{
	GENERATED_BODY()
	
public:
	AMonsterArmsItem();
	virtual void OnPrimaryAnimationClimax(bool FromFirstPersonInstance) override;

};
