// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateWorldWidget.generated.h"

class UButton;
class UEditableTextBox;

UCLASS()
class WILDOMISSION_API UCreateWorldWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

private:	
	UPROPERTY(Meta = (BindWidget))
	UButton* CreateWorldButton;
	
	UPROPERTY(Meta = (BindWidget))
	UButton* BackButton;
	
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* WorldNameInputBox;
	
	UFUNCTION()
	void CreateWorld();
	
	UFUNCTION()
	void WorldNameOnTextChanged(const FText& Text);
	
};
