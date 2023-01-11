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
	UGameplayMenuWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void Setup();
	
	UFUNCTION(BlueprintCallable)
	void Teardown();

	UFUNCTION(BlueprintCallable)
	bool IsOpen() const;

private:
	bool bOpen;

};
