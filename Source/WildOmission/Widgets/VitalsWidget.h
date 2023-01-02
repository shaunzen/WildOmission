// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VitalsWidget.generated.h"

class UProgressBar;
class UVitalsComponent;

UCLASS()
class WILDOMISSION_API UVitalsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Set(UVitalsComponent* InVitals);

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ThirstBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HungerBar;
};
