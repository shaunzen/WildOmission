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
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SaveNameText;

	UPROPERTY(meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitToMenuButton;
	
	bool bOpen;

	UFUNCTION()
	void Teardown();

	UFUNCTION()
	void SaveGame();
	
	UFUNCTION()
	void QuitToMenu();

};
