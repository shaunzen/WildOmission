// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "WildOmissionCharacter.generated.h"

class UCameraComponent;
class UVitalsComponent;
class UInventoryManipulatorComponent;
class UPlayerInventoryComponent;
class UInteractionComponent;
class UPlayerHUDWidget;
class UInputAction;
class UInputMappingContext;
class UEquipComponent;
class AEquipableItem;

UCLASS()
class WILDOMISSION_API AWildOmissionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWildOmissionCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsLocalPlayer() const;

	UFUNCTION()
	USkeletalMeshComponent* GetFirstPersonMesh();

	void PlaySwingAnimation();

	UVitalsComponent* GetVitalsComponent();
	
	// Returns players inventory component
	UPlayerInventoryComponent* GetInventoryComponent();

	// Returns players equip component
	UFUNCTION(BlueprintCallable)
	UEquipComponent* GetEquipComponent() const;

	UPlayerHUDWidget* GetHUD();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(VisibleAnywhere)
	UEquipComponent* EquipComponent;

	UPROPERTY(VisibleAnywhere)
	UVitalsComponent* VitalsComponent;
	
	UPROPERTY(VisibleAnywhere)
	UInventoryManipulatorComponent* InventoryManipulator;

	UPROPERTY(VisibleAnywhere)
	UPlayerInventoryComponent* InventoryComponent;
	
	UPROPERTY(VisibleAnywhere)
	UInteractionComponent* InteractionComponent;

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
	
};
