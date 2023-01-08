// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRowWidget.generated.h"

class UTextBlock;
class UButton;
class UMainMenuWidget;

UCLASS()
class WILDOMISSION_API UServerRowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerName;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HostUser;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ConnectionFraction;

	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;

	void Setup(UMainMenuWidget* InParent, uint32 InIndex);

private:
	UPROPERTY(meta = (BindWidget))
	UButton* RowButton;

	UPROPERTY()
	UMainMenuWidget* Parent;

	uint32 Index;

	UFUNCTION()
	void OnClicked();

};
