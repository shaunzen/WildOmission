// Copyright Telephone Studios. All Rights Reserved.


#include "Components/BuilderComponent.h"
#include "UI/ToolCupboardWidget.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UBuilderComponent::UBuilderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ToolCupboardWidgetClass = nullptr;
	ToolCupboardWidget = nullptr;

	static ConstructorHelpers::FClassFinder<UToolCupboardWidget> ToolCupboardWidgetBlueprint(TEXT("/Game/Deployables/UI/WBP_ToolCupboardMenu"));
	if (ToolCupboardWidgetBlueprint.Succeeded())
	{
		ToolCupboardWidgetClass = ToolCupboardWidgetBlueprint.Class;
	}
}

void UBuilderComponent::OpenToolCupboardMenu(AToolCupboard* ToolCupboard)
{
	Client_OpenToolCupboardMenu(ToolCupboard);
}


// Called when the game starts
void UBuilderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UBuilderComponent::Client_OpenToolCupboardMenu_Implementation(AToolCupboard* ToolCupboard)
{
	UWorld* World = GetWorld();
	if (World == nullptr || ToolCupboard == nullptr || ToolCupboardWidgetClass == nullptr || ToolCupboardWidget != nullptr)
	{
		return;
	}

	ToolCupboardWidget = CreateWidget<UToolCupboardWidget>(World, ToolCupboardWidgetClass);
	if (ToolCupboardWidget == nullptr)
	{
		return;
	}

	ToolCupboardWidget->Setup(ToolCupboard);
}
