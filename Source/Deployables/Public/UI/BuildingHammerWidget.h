// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingHammerWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildingHammerWidgetTeardownSignature);

class ADeployable;

UCLASS()
class DEPLOYABLES_API UBuildingHammerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Setup(ADeployable* Deployable); // TODO take in deployable to modify
	void Teardown();
	
	FOnBuildingHammerWidgetTeardownSignature OnTeardown;

protected:
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	void SetMouseCursorToCenter();

};
