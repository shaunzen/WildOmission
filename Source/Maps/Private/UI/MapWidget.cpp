// Copyright Telephone Studios. All Rights Reserved.


#include "UI/MapWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Interfaces/MapInterface.h"

UMapWidget::UMapWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SetIsFocusable(true);
	CloseButton = nullptr;
}

void UMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UMapWidget::Teardown);
	}

	if (SaveCurrentLocationButton)
	{
		SaveCurrentLocationButton->OnClicked.AddDynamic(this, &UMapWidget::SaveCurrentLocationButtonOnClicked);
	}
}

void UMapWidget::Setup()
{
	this->AddToViewport();

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	FInputModeUIOnly InputData;
	InputData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetShowMouseCursor(true);
	PlayerController->SetInputMode(InputData);

	this->SetFocus();

	// Update the coordinates
	RefreshCoordinates();
}

void UMapWidget::Teardown()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	FInputModeGameOnly InputData;
	PlayerController->SetShowMouseCursor(false);
	PlayerController->SetInputMode(InputData);

	if (OnTeardown.IsBound())
	{
		OnTeardown.Broadcast();
	}
	
	this->RemoveFromParent();
}

FReply UMapWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		this->Teardown();
	}

	return FReply::Handled();
}

void UMapWidget::SaveCurrentLocationButtonOnClicked()
{	
	// Get the map interface
	IMapInterface* MapInterface = GetOwnerAsMapInterface();
	if (MapInterface == nullptr)
	{
		return;
	}

	// Update the saved location on the owning player
	MapInterface->SetSavedLocationToCurrentLocation();

	RefreshCoordinates();
}

void UMapWidget::RefreshCoordinates()
{
	IMapInterface* MapInterface = GetOwnerAsMapInterface();
	if (MapInterface == nullptr || CurrentLocationTextBlock == nullptr
		|| LastDeathLocationTextBlock == nullptr || SavedLocationTextBlock == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to refresh map coordinates, map interface nullptr."));
		return;
	}

	const FVector CurrentLocation = MapInterface->GetCurrentCoordinateLocation();
	const FVector LastDeathLocation = MapInterface->GetLastDeathLocation();
	const FVector SavedLocation = MapInterface->GetSavedLocation();

	const FString CurrentLocationString =
		FString::Printf(TEXT("Current Location: X = %i, Y = %i, Z = %i"),
			FMath::RoundToInt32(CurrentLocation.X),
			FMath::RoundToInt32(CurrentLocation.Y),
			FMath::RoundToInt32(CurrentLocation.Z));
	const FString LastDeathLocationString =
		FString::Printf(TEXT("Last Death Location: X = %i, Y = %i, Z = %i"),
			FMath::RoundToInt32(LastDeathLocation.X),
			FMath::RoundToInt32(LastDeathLocation.Y),
			FMath::RoundToInt32(LastDeathLocation.Z));
	const FString SavedLocationString =
		FString::Printf(TEXT("Saved Location: X = %i, Y = %i, Z = %i"),
			FMath::RoundToInt32(SavedLocation.X),
			FMath::RoundToInt32(SavedLocation.Y),
			FMath::RoundToInt32(SavedLocation.Z));

	CurrentLocationTextBlock->SetText(FText::FromString(CurrentLocationString));
	LastDeathLocationTextBlock->SetText(FText::FromString(LastDeathLocationString));
	SavedLocationTextBlock->SetText(FText::FromString(SavedLocationString));
}

IMapInterface* UMapWidget::GetOwnerAsMapInterface() const
{
	// Get the owning player controller
	APlayerController* OwnerPlayerController = GetOwningPlayer();
	if (!IsValid(OwnerPlayerController))
	{
		return nullptr;
	}

	IMapInterface* MapInterface = Cast<IMapInterface>(OwnerPlayerController);
	return MapInterface;
}
