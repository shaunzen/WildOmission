// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingHammerWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildingHammerWidgetTeardownSignature);

class UTextBlock;
class ADeployable;
class ABuildingHammerItem;
class ABuildingBlock;

UCLASS()
class DEPLOYABLES_API UBuildingHammerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Show(ABuildingHammerItem* BuildingHammer, ADeployable* InDeployable);
	void Teardown();
	
	FOnBuildingHammerWidgetTeardownSignature OnTeardown;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* UpgradePanel;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* UpgradeTextBlock;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* UpgradeCostTextBlock;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* UpgradeHasTextBlock;

	UPROPERTY(Meta = (BindWidget))
	UPanelWidget* DestroyPanel;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* DestroyTextBlock;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* DestroyRefundTextBlock;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* ButtonSound;
	
	UPROPERTY()
	ABuildingHammerItem* OwnerBuildingHammer;
	UPROPERTY()
	ADeployable* Deployable;

	void ScaleWidgetByBool(UWidget* WidgetToScale, bool Increase);

	bool UpgradeSelected;
	bool DestroySelected;

	bool CanPlayerAffordUpgrade() const;

	void SetupUpgradeText();
	void SetupDestroyText();

	void SetMouseCursorToCenter();

};
