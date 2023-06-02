// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRowWidget.generated.h"

class UTextBlock;
class UButton;
class UBorder;
class UServerBrowserWidget;

UCLASS()
class WILDOMISSION_API UServerRowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerName;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HostUser;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ConnectionFraction;

	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;

	void Setup(UServerBrowserWidget* InParent, uint32 InIndex);
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* RowButton;

	UPROPERTY(Meta = (BindWidget))
	UBorder* RowBorder;
	
	UPROPERTY()
	UServerBrowserWidget* Parent;

	uint32 Index;

	UPROPERTY()
	bool Hovering = false;

	UFUNCTION()
	void OnClicked();

};
