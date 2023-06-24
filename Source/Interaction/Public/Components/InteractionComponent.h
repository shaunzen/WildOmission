// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTERACTION_API UInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void Interact();

	UFUNCTION(BlueprintCallable)
	FString GetInteractionString() const;

private:

	UPROPERTY(EditDefaultsOnly)
	float InteractionRange;

	UPROPERTY(EditDefaultsOnly)
	float InteractionRadius;

	FString InteractionString;

	void UpdateInteractionPrompt();

	bool LineTraceOnVisibility(FHitResult& OutHitResult) const;
	
	UFUNCTION(Server, Reliable)
	void Server_Interact(AActor* ActorToInteract);
};
