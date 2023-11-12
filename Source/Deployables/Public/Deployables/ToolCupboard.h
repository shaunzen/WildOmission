// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployables/Deployable.h"
#include "Interfaces/Interactable.h"
#include "ToolCupboard.generated.h"

/**
 * 
 */
UCLASS()
class DEPLOYABLES_API AToolCupboard : public ADeployable, public IInteractable
{
	GENERATED_BODY()
	
public:
	AToolCupboard();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnSpawn();

	// Begin IInteractable Interface
	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;
	// End IInteractable Interface

	void AuthorizePlayer(const FString& PlayerUniqueID);
	void DeauthorizePlayer(const FString& PlayerUniqueID);
	bool IsPlayerAuthorized(const FString& PlayerUniqueID) const;
	void ClearAuthorizedPlayers();

	bool IsWithinRange(const FVector& LocationToTest) const;

	static TArray<AToolCupboard*> GetAllToolCupboards();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(VisibleAnywhere)
	class ULockComponent* LockComponent;

	UPROPERTY(EditDefaultsOnly)
	float Range;
	
	UPROPERTY(VisibleAnywhere, Replicated, SaveGame)
	TArray<FString> AuthorizedPlayers;

	void OnLoadComplete_Implementation();

};
