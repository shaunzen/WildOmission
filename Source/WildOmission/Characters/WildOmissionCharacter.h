// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "WildOmissionCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UCraftingComponent;
class UNameTagComponent;
class AItemContainerBase;
class UWeatherEffectHandlerComponent;

UCLASS()
class WILDOMISSION_API AWildOmissionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWildOmissionCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	void HandleDeath();

	UFUNCTION(Client, Reliable)
	void Client_OpenContainer(AItemContainerBase* Container);
	
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
	USpringArmComponent* FirstPersonSpringArmComponent;

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

	UPROPERTY(VisibleAnywhere)
	UWeatherEffectHandlerComponent* WeatherEffectHandler;

	UPROPERTY()
	class UPlayerHUDWidget* PlayerHUDWidget;
	TSubclassOf<class UPlayerHUDWidget> PlayerHUDWidgetClass;

	UPROPERTY()
	TSubclassOf<AItemContainerBase> RagdollClass;

	UPROPERTY()
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
	
	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);

	void StartSprint();

	void EndSprint();

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_MovementSpeed)
	float DesiredMovementSpeed;

	UFUNCTION()
	void OnRep_MovementSpeed();

	UFUNCTION(Server, Reliable)
	void Server_Sprint(bool bShouldSprint);

	void PrimaryPressed();
	void PrimaryReleased();

	void SecondaryPressed();
	void SecondaryReleased();

	void ToggleInventoryMenu();
	
	void ToggleCraftingMenu();

	void ToggleChat();

	void ToolbarSelectionIncrement();
	
	void ToolbarSelectionDecrement();

	void SelectToolbarSlot1();
	void SelectToolbarSlot2();
	void SelectToolbarSlot3();
	void SelectToolbarSlot4();
	void SelectToolbarSlot5();
	void SelectToolbarSlot6();

	//********************************	
	// Setup Member functions
	//********************************

	void SetupEnhancedInputSubsystem();
	
	void SetupMesh();
	
	void SetupPlayerHUD();

	void SetupWeatherEffectHandler();

	UFUNCTION(NetMulticast, Unreliable)
	void Client_UpdateReplicatedControlRotation(const FRotator& NewControlRotation);

};
