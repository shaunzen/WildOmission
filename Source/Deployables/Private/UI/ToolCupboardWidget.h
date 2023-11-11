// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ToolCupboardWidget.generated.h"

/**
 * 
 */
UCLASS()
class UToolCupboardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UToolCupboardWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void Setup(class AToolCupboard* InToolCupboard);
	void Teardown();

private:
	UPROPERTY(Meta = (BindWidget))
	class UButton* AuthorizeButton;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* AuthorizeTextBlock;

	UPROPERTY(Meta = (BindWidget))
	class UButton* ClearAuthorizedButton;
	
	UPROPERTY(Meta = (BindWidget))
	class UButton* CloseButton;

	UPROPERTY()
	class AToolCupboard* ToolCupboard;

	UFUNCTION()
	void OnAuthorizeButtonClicked();

	UFUNCTION()
	void OnClearAuthorizedButtonClicked();

};
