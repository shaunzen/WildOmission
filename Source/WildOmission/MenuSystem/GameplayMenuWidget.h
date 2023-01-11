// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSION_API UGameplayMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void Setup();
	
	UFUNCTION(BlueprintCallable)
	void Teardown();

};
