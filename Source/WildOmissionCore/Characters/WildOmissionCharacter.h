// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/CharacterSettingsUser.h"
#include "WildOmissionCharacter.generated.h"

class UInputAction;
struct FInputActionValue;
class UInputComponent;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UVitalsComponent;
class UInteractionComponent;
class UPlayerInventoryComponent;
class UInventoryManipulatorComponent;
class UEquipComponent;
class UCraftingComponent;
class UNameTagComponent;
class USpecialEffectsHandlerComponent;

class AItemContainerBase;
class UPlayerHUDWidget;

UCLASS()
class WILDOMISSIONCORE_API AWildOmissionCharacter : public ACharacter, public ICharacterSettingsUser
{
	GENERATED_BODY()

public:
	AWildOmissionCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void Landed(const FHitResult& HitResult) override;

	// Begin ICharacterSettingsUser Implementation
	virtual void ApplyFieldOfView() override;
	// End ICharacterSettingsUser Implementation

	UFUNCTION()
	void HandleDeath();
	
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetArmsMesh() const;

	UFUNCTION()
	float GetHeadPitch() const;

	UFUNCTION()
	FRotator GetReplicatedControlRotation() const;

	UFUNCTION()
	bool IsUnderwater() const;

	UFUNCTION()
	UCameraComponent* GetFirstPersonCameraComponent();

	UFUNCTION(BlueprintCallable)
	UPlayerHUDWidget* GetHUDWidget() const;

	UFUNCTION(BlueprintCallable)
	UVitalsComponent* GetVitalsComponent() const;

	UFUNCTION(BlueprintCallable)
	UInventoryManipulatorComponent* GetInventoryManipulatorComponent() const;

	UFUNCTION(BlueprintCallable)
	UPlayerInventoryComponent* GetInventoryComponent() const;

	UFUNCTION(BlueprintCallable)
	UCraftingComponent* GetCraftingComponent() const;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* FirstPersonArmsMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* FirstPersonSpringArmComponent;


	UPROPERTY(VisibleAnywhere)
	UVitalsComponent* VitalsComponent;

	UPROPERTY(VisibleAnywhere)
	UInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere)
	UPlayerInventoryComponent* InventoryComponent;
	
	UPROPERTY(VisibleAnywhere)
	UInventoryManipulatorComponent* InventoryManipulatorComponent;

	UPROPERTY(VisibleAnywhere)
	UEquipComponent* EquipComponent;
	
	UPROPERTY(VisibleAnywhere)
	UCraftingComponent* CraftingComponent;

	UPROPERTY(VisibleAnywhere)
	UNameTagComponent* NameTag;

	UPROPERTY(VisibleAnywhere)
	USpecialEffectsHandlerComponent* SpecialEffectsHandlerComponent;

	UPROPERTY()
	UPlayerHUDWidget* PlayerHUDWidget;
	TSubclassOf<UPlayerHUDWidget> PlayerHUDWidgetClass;

	UPROPERTY()
	TSubclassOf<AItemContainerBase> RagdollClass;

	UPROPERTY(Replicated)
	FRotator ReplicatedControlRotation;

	UPROPERTY()
	bool bSprinting;

	UPROPERTY()
	bool bUnderwater;

	void HandleUnderwater();

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
	UInputAction* SprintAction;

	UPROPERTY()
	UInputAction* JumpAction;
	
	UPROPERTY()
	UInputAction* InteractAction;
	
	UPROPERTY()
	UInputAction* PrimaryAction;
	
	UPROPERTY()
	UInputAction* SecondaryAction;

	UPROPERTY()
	UInputAction* ReloadAction;
	
	UPROPERTY()
	UInputAction* ToggleInventoryMenuAction;

	UPROPERTY()
	UInputAction* ToggleCraftingMenuAction;

	UPROPERTY()
	UInputAction* ToggleChatAction;
	
	UPROPERTY()
	UInputAction* ToolbarSelectionIncrementAction;
	
	UPROPERTY()
	UInputAction* ToolbarSelectionDecrementAction;

	UPROPERTY()
	UInputAction* ToolbarSelection1Action;
	
	UPROPERTY()
	UInputAction* ToolbarSelection2Action;
	
	UPROPERTY()
	UInputAction* ToolbarSelection3Action;
	
	UPROPERTY()
	UInputAction* ToolbarSelection4Action;
	
	UPROPERTY()
	UInputAction* ToolbarSelection5Action;
	
	UPROPERTY()
	UInputAction* ToolbarSelection6Action;
	
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void StartSprint();
	UFUNCTION()
	void EndSprint();

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_MovementSpeed)
	float DesiredMovementSpeed;
	
	UFUNCTION()
	void OnRep_MovementSpeed();

	UFUNCTION(Server, Reliable)
	void Server_Sprint(bool bShouldSprint);

	UFUNCTION()
	void PrimaryPressed();
	UFUNCTION()
	void PrimaryReleased();

	UFUNCTION()
	void SecondaryPressed();
	UFUNCTION()
	void SecondaryReleased();

	UFUNCTION()
	void ReloadPressed();

	UFUNCTION()
	void ToggleInventoryMenu();
	
	UFUNCTION()
	void ToggleCraftingMenu();

	UFUNCTION()
	void ToggleChat();

	UFUNCTION()
	void ToolbarSelectionIncrement();
	
	UFUNCTION()
	void ToolbarSelectionDecrement();

	UFUNCTION()
	void SelectToolbarSlot1();
	UFUNCTION()
	void SelectToolbarSlot2();
	UFUNCTION()
	void SelectToolbarSlot3();
	UFUNCTION()
	void SelectToolbarSlot4();
	UFUNCTION()
	void SelectToolbarSlot5();
	UFUNCTION()
	void SelectToolbarSlot6();

	//********************************	
	// Setup Member functions
	//********************************

	void SetupEnhancedInputSubsystem();
	
	void SetupMesh();
	
	void SetupPlayerHUD();

	void SetupWeatherEffectHandler();

};
