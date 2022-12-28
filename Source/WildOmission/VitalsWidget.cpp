// (c) 2023 Telephone Studios, all rights reserved.


#include "VitalsWidget.h"
#include "Components/ProgressBar.h"
#include "VitalsComponent.h"

UVitalsWidget::UVitalsWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{

}

void UVitalsWidget::Setup()
{

}

void UVitalsWidget::SetVitals(UVitalsComponent* InVitals)
{

}

bool UVitalsWidget::Initialize()
{
	bool Success = Super::Initialize();
	return Success;
}