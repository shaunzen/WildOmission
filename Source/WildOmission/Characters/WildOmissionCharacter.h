// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "WildOmissionCharacter.generated.h"

class UCameraComponent;
class UCraftingComponent;
class UNameTagComponent;
class AContainerBase;

UCLASS()
class WILDOMISSION_API AWildOmissionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWildOmissionCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	void HandleDeath();

	void StartSwimming();
	
	void StopSwimming();

	UFUNCTION(Client, Reliable)
	void Client_OpenContainer(AContainerBase* Container);
	
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetArmsMesh() const;

	UFUNCTION()
	float GetHeadPitch() const;

	UFUNCTION()
	UCameraComponent* GetFirstPersonCameraComponent();

	UFUNCTION(BlueprintCallable)
	class UPlayerHUDWidget* GetHUDWidget() const;

	UFUNCTION(BlueprintCallable)
	class UVitalsComponent* GetVitalsComponent() const;

	UFUNCTION(BlueprintCallable)
	class UInventoryManipulatorComponent* GetInventoryManipulatorComponent() const;

	UFUNCTION(BlueprintCallable)
	class UPlayerInventoryComponent* GetInventoryComponent() const;

	UFUNCTION(BlueprintCallable)
	UCraftingComponent* GetCraftingComponent() const;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* FirstPersonArmsMeshComponent;

	UPROPERTY(VisibleAnywhere)
	class UEquipComponent* EquipComponent;

	UPROPERTY(VisibleAnywhere)
	class UVitalsComponent* VitalsComponent;
	
	UPROPERTY(VisibleAnywhere)
	class UInventoryManipulatorComponent* InventoryManipulatorComponent;

	UPROPERTY(VisibleAnywhere)
	class UPlayerInventoryComponent* InventoryComponent;
	
	UPROPERTY(VisibleAnywhere)
	UCraftingComponent* CraftingComponent;

	UPROPERTY(VisibleAnywhere)
	class UInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere)
	UNameTagComponent* NameTag;

	UPROPERTY()
	class UPlayerHUDWidget* PlayerHUDWidget;
	TSubclassOf<class UPlayerHUDWidget> PlayerHUDWidgetClass;

	UPROPERTY()
	float HeadPitch;

	bool bIsSwimming;

	//*****************************
	// Input
	//*****************************
	UPROPERTY()
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY()
	class UInputAction* MoveAction;

	UPROPERTY()
	class UInputAction* LookAction;
	
	UPROPERTY()
	class UInputAction* JumpAction;
	
	UPROPERTY()
	class UInputAction* InteractAction;
	
	UPROPERTY()
	class UInputAction* PrimaryAction;
	
	UPROPERTY()
	class UInputAction* SecondaryAction;
	
	UPROPERTY()
	class UInputAction* ToggleInventoryMenuAction;

	UPROPERTY()
	UInputAction* ToggleCraftingMenuAction;
	
	UPROPERTY()
	class UInputAction* ToolbarSelectionIncrementAction;
	
	UPROPERTY()
	class UInputAction* ToolbarSelectionDecrementAction;
	
	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);

	void Primary();
	
	void Secondary();

	void ToggleInventoryMenu();
	
	void ToggleCraftingMenu();

	void ToolbarSelectionIncrement();
	
	void ToolbarSelectionDecrement();

	//********************************	
	// Setup Member functions
	//********************************

	void SetupEnhancedInputSubsystem();
	
	void SetupMesh();
	
	void SetupPlayerHUD();

	UFUNCTION(NetMulticast, Unreliable)
	void Client_UpdateHeadPitch(const float& NewHeadPitch);

};
