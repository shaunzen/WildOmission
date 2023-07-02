// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DurabilityBarWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class DEPLOYABLES_API UDurabilityBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* DurabilityNameTextBlock;
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* DurabilityFractionTextBlock;
	
	UPROPERTY(Meta = (BindWidget))
	UProgressBar* DurabilityBar;
	
};
