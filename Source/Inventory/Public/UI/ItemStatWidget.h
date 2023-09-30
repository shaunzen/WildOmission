// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemStatWidget.generated.h"

struct FItemStat;
class UTextBlock;

UCLASS()
class INVENTORY_API UItemStatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UItemStatWidget(const FObjectInitializer& ObjectInitializer);
	void Setup(const FItemStat& CurrentStat, const FItemStat& DefaultStat);

private:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* StatNameTextBlock;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* StatValueTextBlock;

};
