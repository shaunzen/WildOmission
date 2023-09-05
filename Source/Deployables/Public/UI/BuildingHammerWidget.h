// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingHammerWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildingHammerWidgetTeardownSignature);

class UTextBlock;
class ADeployable;
class ABuildingBlock;

UCLASS()
class DEPLOYABLES_API UBuildingHammerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Setup(ADeployable* InDeployable);
	void Teardown();
	
	FOnBuildingHammerWidgetTeardownSignature OnTeardown;

protected:
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* UpgradeTextBlock;
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* DestroyTextBlock;

	UPROPERTY()
	ADeployable* Deployable;

	FString GetUpgradeString(ABuildingBlock* BuildingBlock) const;
	void SetMouseCursorToCenter();

};
