// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ToolCupboardWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnToolCupboardWidgetTeardownSignature);

UCLASS()
class UToolCupboardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UToolCupboardWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	FOnToolCupboardWidgetTeardownSignature OnTeardown;

	UFUNCTION()
	void Setup(class AToolCupboard* InToolCupboard);

	UFUNCTION()
	void Teardown();

protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

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

	bool AreWeAuthorized() const;

	UFUNCTION()
	void RefreshUI();

	UFUNCTION()
	void OnAuthorizeButtonClicked();

	UFUNCTION()
	void OnClearAuthorizedButtonClicked();

};
