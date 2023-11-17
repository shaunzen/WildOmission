// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRowWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnServerRowSelectedSignature, const uint32&, SelectedIndex);

UCLASS()
class UServerRowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UServerRowWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;

	void Setup(const uint32& InIndex, const struct FServerData& ServerData);
	
	void SetSelected(bool NewSelected);

	FOnServerRowSelectedSignature OnSelected;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(Meta = (BindWidget))
	class UButton* RowButton;

	UPROPERTY(Meta = (BindWidget))
	class UBorder* RowBorder;
	
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* ServerNameTextBlock;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* HostNameTextBlock;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* ConnectionFractionTextBlock;

	UPROPERTY()
	uint32 Index;

	UPROPERTY()
	bool Selected;

	UPROPERTY()
	bool Hovering;

	UFUNCTION()
	void OnClicked();

};
