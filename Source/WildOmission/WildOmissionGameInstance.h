// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WildOmissionGameInstance.generated.h"

class UMainMenuWidget;

UCLASS()
class WILDOMISSION_API UWildOmissionGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UWildOmissionGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void ShowMainMenuWidget();
private:

	TSubclassOf<UMainMenuWidget> MainMenuWidgetBlueprintClass;
	UMainMenuWidget* MainMenuWidget;
};
