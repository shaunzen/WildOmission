// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployables/Deployable.h"
#include "Interfaces/Interactable.h"
#include "Sign.generated.h"

/**
 * 
 */
UCLASS()
class DEPLOYABLES_API ASign : public ADeployable, public IInteractable
{
	GENERATED_BODY()
	
public:
	ASign();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Begin IInteractable Implementation
	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;
	// End IInteractable Implementation

	void SetText(const FString& NewText);
	FString GetText() const;

private:
	UPROPERTY(VisibleAnywhere)
	class UTextRenderComponent* TextRenderComponent;

	UPROPERTY(SaveGame, Replicated, ReplicatedUsing = OnRep_Text)
	FString Text;

	UFUNCTION()
	void OnRep_Text();

	virtual void OnLoadComplete_Implementation() override;

};
