// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingWidget.generated.h"

class UButton;

UCLASS()
class WILDOMISSION_API UCraftingWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* CraftPickaxeButton;

	UPROPERTY(Meta = (BindWidget))
	UButton* CraftHatchetButton;

	UFUNCTION()
	void PickaxeButtonClicked();
	UFUNCTION()
	void HatchetButtonClicked();

};
