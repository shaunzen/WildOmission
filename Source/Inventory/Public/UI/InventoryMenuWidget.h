// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMenuWidget.generated.h"

class UHorizontalBox;
class UInventoryWidget;
class UPlayerInventoryWidget;
class USelectedItemWidget;
class UHoveredItemNameTag;

class UInventoryComponent;

UCLASS()
class INVENTORY_API UInventoryMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventoryMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeomentry, float InDeltaTime) override;

	void Open();
	void Close(bool ShouldCloseContainer = false);
	void OpenContainer(UInventoryComponent* ContainerInventoryComponent, UClass* ContainerWidgetClass);

	bool IsOpen() const;
	bool IsShiftHeld() const;
	UPlayerInventoryWidget* GetPlayerInventoryWidget() const;
	USelectedItemWidget* GetSelectedItemWidget() const;
	UHoveredItemNameTag* GetHoveredItemNameTagWidget() const;
	bool SelectedItemVisible() const;

protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

private:
	UPROPERTY(Meta = (BindWidget))
	UHorizontalBox* InventoryHorizontalBox;
	
	UPROPERTY(Meta = (BindWidget))
	UPlayerInventoryWidget* PlayerInventoryWidget;

	UPROPERTY()
	UInventoryWidget* OpenContainerWidget;

	UPROPERTY(Meta = (BindWidget))
	USelectedItemWidget* SelectedItemWidget;

	UPROPERTY(Meta = (BindWidget))
	UHoveredItemNameTag* HoveredItemNameTagWidget;

	UPROPERTY()
	bool ShiftHeld;

	void UpdateFollowMousePointerWidgets();

};
