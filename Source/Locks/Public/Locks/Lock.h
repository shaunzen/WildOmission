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

	FString GetCode() const;
	bool IsLocked() const;
	bool IsPlayerAuthorized(const FString& PlayerUniqueID) const;

	UStaticMesh* GetStaticMesh() const;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	class ULockComponent* OwnerLockComponent;

};
