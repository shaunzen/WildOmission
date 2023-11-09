// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "Lock.generated.h"

UCLASS()
class LOCKS_API ALock : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:
	ALock();
	
	void Setup(class ULockComponent* InOwnerLockComponent);

	// Begin IInteractable Implementation
	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;
	// End IInteractable Implementation

	void SetCode(const FString& NewCode, const FString& CodeSetterUniqueID);
	void ClearCode();
	FString GetCode() const;
	bool IsLocked() const;
	void AuthorizePlayer(const FString& PlayerUniqueID);
	bool IsAuthorized(const FString& PlayerUniqueID) const;
	bool IsAuthorized(class ULockModifierComponent* LockModifier) const;
	bool IsAuthorized(APawn* PlayerPawn) const;

	UStaticMesh* GetStaticMesh() const;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	class ULockComponent* OwnerLockComponent;

};
