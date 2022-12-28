// (c) 2023 Telephone Studios, all rights reserved.

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
	UVitalsWidget(const FObjectInitializer& ObjectInitializer);

	void Setup();
	void SetVitals(UVitalsComponent* InVitals);

protected:
	virtual bool Initialize() override;
private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ThirstBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HungerBar;
};
