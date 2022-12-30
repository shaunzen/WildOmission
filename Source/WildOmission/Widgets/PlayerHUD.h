// (c) 2023 Telephone Studios, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class UProgressBar;
class UTextBlock;
class UVitalsComponent;

UCLASS(Abstract)
class WILDOMISSION_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetInteractionPrompt(FString InString);
	void SetVitals(UVitalsComponent* InVitals);
private:
	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
		UProgressBar* ThirstBar;
	UPROPERTY(meta = (BindWidget))
		UProgressBar* HungerBar;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* InteractionPrompt;
};