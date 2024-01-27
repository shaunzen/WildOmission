// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VitalsWidget.generated.h"

class UProgressBar;

UCLASS()
class VITALS_API UVitalsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ThirstBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HungerBar;

	void UpdateBars();

};
