// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
};

UCLASS()
class WILDOMISSION_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	void Setup();
	void Teardown();

protected:
	virtual bool Initialize() override;
};
