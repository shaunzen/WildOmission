// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerRowWidget.generated.h"

class UTextBlock;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerRowOnRequestRefreshSignature);

UCLASS()
class UPlayerRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerRowWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void Setup(const FString& Name, const FString& InUniqueID);

	FPlayerRowOnRequestRefreshSignature OnRequestRefresh;

private:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* NameTextBlock;

	UPROPERTY(Meta = (BindWidget))
	UButton* ViewProfileButton;

	UPROPERTY(Meta = (BindWidget))
	UButton* KickButton;

	UPROPERTY()
	FString UniqueID;

	UFUNCTION()
	void ViewProfile();

	UFUNCTION()
	void Kick();

	APlayerController* GetPlayerControllerForThis() const;
};
