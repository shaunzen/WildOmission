// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingMenuWidget.generated.h"

class UButton;

UCLASS()
class WILDOMISSION_API UCraftingMenuWidget : public UUserWidget
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
