// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockModifierComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOCKS_API ULockModifierComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockModifierComponent();

	void OpenKeypadMenu(class ALock* Lock);

	FString GetOwnerUniqueID() const;

private:
	UPROPERTY()
	class UKeypadWidget* KeypadWidget;

	UPROPERTY()
	TSubclassOf<class UKeypadWidget> KeypadWidgetClass;

	UFUNCTION(Client, Reliable)
	void Client_OpenKeypadMenu(class ALock* Lock, TEnumAsByte<ELockOperation> LockOperation);

	UFUNCTION()
	void OnKeypadTeardown();

};
