// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingMenuWidget.generated.h"

class UTileView;

UCLASS()
class WILDOMISSION_API UCraftingMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void OnOpen();

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(Meta = (BindWidget))
	UTileView* RecipeTileView;
	
	UFUNCTION()
		void Craft();
};
