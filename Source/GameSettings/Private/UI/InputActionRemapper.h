// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputActionRemapper.generated.h"

class UTextBlock;
class UKeyOptionBox;
class UInputAction;
class UInputMappingContext;

UCLASS()
class UInputActionRemapper : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetInputAction(UInputAction* InInputAction);
	UInputAction* GetInputAction() const;

	UKeyOptionBox* GetKeyOptionBox() const;

private:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ActionTextBlock;

	UPROPERTY(Meta = (BindWidget))
	UKeyOptionBox* KeyOptionBox;

	UPROPERTY()
	UInputAction* InputAction;
	
};
