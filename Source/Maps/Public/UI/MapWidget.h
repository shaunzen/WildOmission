// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMapWidgetTeardownSignature);

UCLASS()
class MAPS_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMapWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
	void Setup();

	UFUNCTION()
	void Teardown();

	FOnMapWidgetTeardownSignature OnTeardown;

protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

private:
	UPROPERTY(Meta = (BindWidget))
	class UButton* CloseButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* SaveCurrentLocationButton;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* CurrentLocationTextBlock;
	
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* LastDeathLocationTextBlock;
	
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* SavedLocationTextBlock;

	UFUNCTION()
	void SaveCurrentLocationButtonOnClicked();

	UFUNCTION()
	void RefreshCoordinates();

	class IMapInterface* GetOwnerAsMapInterface() const;

};
