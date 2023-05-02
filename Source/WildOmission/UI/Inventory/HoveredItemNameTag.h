// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HoveredItemNameTag.generated.h"

class UTextBlock;

UCLASS()
class WILDOMISSION_API UHoveredItemNameTag : public UUserWidget
{
	GENERATED_BODY()
public:
	UHoveredItemNameTag(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void Show(const FString& ItemName);
	void Hide();

private:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ItemNameTextBlock;

};
