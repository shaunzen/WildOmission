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

	// Called when the item is equiped into the players hands. Note: Only Called On Server!
	virtual void Equip(APawn* InOwnerPawn, USkeletalMeshComponent* InThirdPersonMeshComponent, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID);

	virtual void OnPrimaryHeld() override;

};
