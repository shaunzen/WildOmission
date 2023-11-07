// Copyright Telephone Studios. All Rights Reserved.


#include "Components/LockModifierComponent.h"
#include "Locks/Lock.h"
#include "UI/KeypadWidget.h"

// Sets default values for this component's properties
ULockModifierComponent::ULockModifierComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UKeypadWidget> KeypadWidgetBlueprint(TEXT("/Game/Deployables/UI/WBP_KeypadMenu"));
	if (KeypadWidgetBlueprint.Succeeded())
	{
		KeypadWidgetClass = KeypadWidgetBlueprint.Class;
	}
}

void ULockModifierComponent::OpenKeypadMenu(ALock* Lock)
{
	Client_OpenKeypadMenu(Lock);
}

void ULockModifierComponent::Client_OpenKeypadMenu_Implementation(ALock* Lock)
{
	UWorld* World = GetWorld();
	if (World == nullptr || KeypadWidget || Lock == nullptr || KeypadWidgetClass == nullptr)
	{
		return;
	}

	KeypadWidget = CreateWidget<UKeypadWidget>(World, KeypadWidgetClass);
	KeypadWidget->Setup(Lock);
}
