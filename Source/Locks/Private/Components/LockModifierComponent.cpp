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

	static ConstructorHelpers::FClassFinder<UKeypadWidget> KeypadWidgetBlueprint(TEXT("/Game/Locks/UI/WBP_KeypadMenu"));
	if (KeypadWidgetBlueprint.Succeeded())
	{
		KeypadWidgetClass = KeypadWidgetBlueprint.Class;
	}
}

void ULockModifierComponent::OpenKeypadMenu(ALock* Lock)
{
	TEnumAsByte<ELockOperation> LockOperation;

	if (Lock->IsPlayerAuthorized(GetOwnerUniqueID()) && Lock->IsLocked())
	{
		LockOperation = ELockOperation::ELO_ModifyCode;
	}
	else if (!Lock->IsPlayerAuthorized(GetOwnerUniqueID()) && Lock->IsLocked())
	{
		LockOperation = ELockOperation::ELO_Authorize;
	}
	else if (!Lock->IsLocked())
	{
		LockOperation = ELockOperation::ELO_SetCode;
	}

	Client_OpenKeypadMenu(Lock, LockOperation);
}

FString ULockModifierComponent::GetOwnerUniqueID() const
{
	// get player state
	// get unique id
	// convert to string

	return FString();
}

void ULockModifierComponent::Client_OpenKeypadMenu_Implementation(ALock* Lock, TEnumAsByte<ELockOperation> LockOperation)
{
	UWorld* World = GetWorld();
	if (World == nullptr || KeypadWidget || Lock == nullptr || KeypadWidgetClass == nullptr)
	{
		return;
	}

	KeypadWidget = CreateWidget<UKeypadWidget>(World, KeypadWidgetClass);
	KeypadWidget->OnTeardown.AddDynamic(this, &ULockModifierComponent::OnKeypadTeardown);
	KeypadWidget->Setup(Lock, LockOperation);
}

void ULockModifierComponent::OnKeypadTeardown()
{
	KeypadWidget = nullptr;
}