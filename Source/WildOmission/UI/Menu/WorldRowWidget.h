// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldRowWidget.generated.h"

class UTextBlock;
class UButton;
class UBorder;
class UWorldSelectionWidget;

UCLASS()
class WILDOMISSION_API UWorldRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WorldNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DateCreated;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DaysPlayed;

	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;

	void Setup(UWorldSelectionWidget* InParent, const FString& InWorldName);
	
	FString GetWorldName();

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
	UWorldSelectionWidget* Parent;

	FString WorldName;

	UPROPERTY()
	bool Hovering = false;

	UFUNCTION()
	void OnClicked();

};
