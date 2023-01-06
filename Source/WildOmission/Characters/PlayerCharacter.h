// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UVitalsComponent;
class UInventoryComponent;
class UInteractionComponent;
class UPlayerHUDWidget;
class UInputAction;
class UInputMappingContext;

UCLASS()
class WILDOMISSION_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UInventoryComponent* GetInventoryComponent();
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FirstPersonCameraComponent;
	UPROPERTY(VisibleAnywhere)
		UVitalsComponent* VitalsComponent;
	UPROPERTY(VisibleAnywhere)
		UInventoryComponent* InventoryComponent;
	UPROPERTY(VisibleAnywhere)
		UInteractionComponent* InteractionComponent;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UPlayerHUDWidget> PlayerHUDWidgetClass;
	UPROPERTY()
		UPlayerHUDWidget* PlayerHUDWidget;

	//*****************************
	// Input
	//*****************************
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputAction* InteractAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
		UInputAction* InventoryAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ToggleInventory();
};
