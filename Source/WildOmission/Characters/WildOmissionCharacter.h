// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "WildOmissionCharacter.generated.h"

class UCameraComponent;
class UVitalsComponent;
class UInventoryComponent;
class UInteractionComponent;
class UPlayerHUDWidget;
class UInputAction;
class UInputMappingContext;
class AEquipableItem;

UCLASS()
class WILDOMISSION_API AWildOmissionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWildOmissionCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void EquipItem(TSubclassOf<AEquipableItem> Item);
	void Disarm();

	UVitalsComponent* GetVitalsComponent();
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
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* EquipMountPoint;
	
	UPROPERTY()
	AEquipableItem* EquipedItem;
	
	UPROPERTY()
	UPlayerHUDWidget* PlayerHUDWidget;
	TSubclassOf<UPlayerHUDWidget> PlayerHUDWidgetClass;

	//*****************************
	// Input
	//*****************************
	UPROPERTY()
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY()
	UInputAction* MoveAction;
	
	UPROPERTY()
	UInputAction* LookAction;
	
	UPROPERTY()
	UInputAction* JumpAction;
	
	UPROPERTY()
	UInputAction* InteractAction;
	
	UPROPERTY()
	UInputAction* PrimaryAction;
	
	UPROPERTY()
	UInputAction* SecondaryAction;
	
	UPROPERTY()
	UInputAction* InventoryAction;

	UPROPERTY()
	UInputAction* ToolbarSelectionIncrementAction;

	UPROPERTY()
	UInputAction* ToolbarSelectionDecrementAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	void Primary();
	void Secondary();

	void ToggleInventory();
	void ToolbarSelectionIncrement();
	void ToolbarSelectionDecrement();

	UFUNCTION(Server, Reliable)
	void Server_Primary();
	
	UFUNCTION(Server, Reliable)
	void Server_Secondary();
	
};
