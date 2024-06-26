// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CheatMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHEATS_API UCheatMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCheatMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;


};
