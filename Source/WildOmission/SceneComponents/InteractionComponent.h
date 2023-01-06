// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractionComponent.generated.h"

class APlayerCharacter;
class APlayerCharacterController;
class UPlayerHUDWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SetHUDWidget(UPlayerHUDWidget* WidgetToUse);
	
	UFUNCTION()
	void Interact();

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly)
	float InteractionRange;

	UPROPERTY()
	APlayerCharacter* OwnerPlayer;
	UPROPERTY()
	APlayerCharacterController* OwnerPlayerController;
	UPROPERTY()
	UPlayerHUDWidget* PlayerHUDWidget;
	
	void UpdateInteractionPrompt();
	bool InteractableItemInRange(FHitResult& OutHitResult) const;
};
