// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/LockOperation.h"
#include "LockModifierComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOCKS_API ULockModifierComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockModifierComponent();

	void OpenKeypadMenu(ALock* Lock);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetLockCode(ALock* Lock, const FString& Code);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ClearLockCode(ALock* Lock);

	UFUNCTION(Server, Reliable)
	void Server_AuthorizeLock(ALock* Lock, const FString& Code);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RemoveLock(ALock* Lock);

	FString GetOwnerUniqueID() const;

private:
	UPROPERTY()
	class UKeypadWidget* KeypadWidget;
	UPROPERTY()
	class ALock* CurrentLock;

	UPROPERTY()
	TSubclassOf<class UKeypadWidget> KeypadWidgetClass;
	
	UFUNCTION(Client, Reliable)
	void Client_OpenKeypadMenu(ALock* Lock, const uint8& LockOperation);

	UFUNCTION()
	void OnKeypadTeardown();

};
