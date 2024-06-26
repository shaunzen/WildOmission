// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CheatItemButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHEATS_API UCheatItemButtonWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UCheatItemButtonWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	void SetItemID(const FName& NewItemID);
	FName GetItemID() const;

private:
	UPROPERTY(VisibleAnywhere)
	FName ItemID;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* ItemDisplayName;

	UPROPERTY(Meta = (BindWidget))
	class UBorder* ItemIcon;

	UPROPERTY(Meta = (BindWidget))
	class UButton* GiveSingleButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* GiveStackButton;

	void GiveItem(bool Stack);

};
