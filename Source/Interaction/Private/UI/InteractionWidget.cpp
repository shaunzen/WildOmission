// Copyright Telephone Studios. All Rights Reserved.


#include "UI/InteractionWidget.h"
#include "Components/TextBlock.h"
#include "Components/InteractionComponent.h"

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (InteractionPromptTextBlock == nullptr)
	{
		return;
	}

	InteractionPromptTextBlock->SetText(FText());
}

void UInteractionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	APawn* OwnerPawn = GetOwningPlayerPawn<APawn>();
	if (OwnerPawn == nullptr)
	{
		return;
	}

	UInteractionComponent* OwnerInteractionComopnent = OwnerPawn->FindComponentByClass<UInteractionComponent>();
	if (OwnerInteractionComopnent == nullptr)
	{
		return;
	}
	
	InteractionPromptTextBlock->SetText(FText::FromString(OwnerInteractionComopnent->GetInteractionString()));
}
