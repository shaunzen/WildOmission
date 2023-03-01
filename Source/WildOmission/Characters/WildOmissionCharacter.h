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
	
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetArmsMesh() const;

	UFUNCTION(BlueprintCallable)
	UPlayerHUDWidget* GetHUDWidget() const;

	UFUNCTION(BlueprintCallable)
	UVitalsComponent* GetVitalsComponent() const;

	UFUNCTION(BlueprintCallable)
	UPlayerInventoryComponent* GetInventoryComponent() const;

	UFUNCTION(BlueprintCallable)
	UInventoryManipulatorComponent* GetInventoryManipulatorComponent() const;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* FirstPersonArmsMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UEquipComponent* EquipComponent;

	UPROPERTY(VisibleAnywhere)
	UVitalsComponent* VitalsComponent;
	
	UPROPERTY(VisibleAnywhere)
	UInventoryManipulatorComponent* InventoryManipulatorComponent;

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

	//********************************	
	// Setup Member functions
	//********************************

	void SetupEnhancedInputSubsystem();
	void SetupMesh();
	void SetupPlayerHUD();

};
