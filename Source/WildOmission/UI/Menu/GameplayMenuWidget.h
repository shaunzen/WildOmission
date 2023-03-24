// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayMenuWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class WILDOMISSION_API UGameplayMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UGameplayMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void Show();
	
	UFUNCTION(BlueprintCallable)
	bool IsOpen() const;

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(Meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* QuitButtonText;
	
	bool bOpen;

	UFUNCTION()
	void Teardown();

	UFUNCTION()
	void Save();
	
	UFUNCTION()
	void QuitToMenu();

	void SetQuitButtonText(bool PlayerHasAuthority);

};
