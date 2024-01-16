// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipableItem.h"
#include "MapItem.generated.h"

/**
 * 
 */
UCLASS()
class MAPS_API AMapItem : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	AMapItem();

	virtual void OnPrimaryPressed() override;
	virtual void OnSecondaryPressed() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UMapWidget> MapWidgetClass;

	UPROPERTY()
	class UMapWidget* OpenWidget;

	void BringUpMap();

};
