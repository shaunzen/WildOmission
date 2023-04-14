// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateWorldButtonWidget.generated.h"

class UButton;

UCLASS()
class WILDOMISSION_API UCreateWorldButtonWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UButton* GetButton() const;

private:
	UPROPERTY(Meta = (BindWidget))
	UButton* Button;
};
