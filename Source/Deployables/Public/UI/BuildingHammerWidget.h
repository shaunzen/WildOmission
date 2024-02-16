// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingHammerWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildingHammerWidgetTeardownSignature);

UCLASS()
class DEPLOYABLES_API UBuildingHammerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Show(class ABuildingHammerItem* BuildingHammer, class ADeployable* InDeployable);
	void Teardown();
	
	FOnBuildingHammerWidgetTeardownSignature OnTeardown;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(Meta = (BindWidget))
	class UPanelWidget* UpgradePanel;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* UpgradeTextBlock;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* UpgradeCostTextBlock;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* UpgradeHasTextBlock;

	UPROPERTY(Meta = (BindWidget))
	class UPanelWidget* DestroyPanel;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* DestroyTextBlock;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* DestroyRefundTextBlock;

	UPROPERTY(Meta = (BindWidget))
	class UPanelWidget* RotatePanel;
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* RotateTextBlock;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* ButtonSound;
	
	UPROPERTY()
	class ABuildingHammerItem* OwnerBuildingHammer;
	UPROPERTY()
	class ADeployable* Deployable;

	void ScaleWidgetByBool(UWidget* WidgetToScale, bool Increase);

	bool UpgradeSelected;
	bool DestroySelected;
	bool RotateSelected;

	void HandleSelection();

	bool CanPlayerAffordUpgrade() const;

	void SetupUpgradeText();
	void SetupDestroyText();
	void SetupRotateText();

	void SetMouseCursorToCenter();

};
