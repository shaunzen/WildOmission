// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractionComponent.generated.h"

class UPlayerHUDWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void Setup(APawn* InOwnerPawn, UPlayerHUDWidget* WidgetToUse);
	
	UFUNCTION()
	void Interact();

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly)
	float InteractionRange;

	UPROPERTY()
	UPlayerHUDWidget* PlayerHUDWidget;

	UPROPERTY()
	APawn* OwnerPawn;

	void UpdateInteractionPrompt();

	bool LineTraceOnInteractableChannel(FHitResult& OutHitResult) const;
	
	UFUNCTION(Server, Reliable)
	void Server_Interact(AActor* ActorToInteract);
};
