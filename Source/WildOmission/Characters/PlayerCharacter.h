// (c) 2023 Telephone Studios, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UVitalsComponent;
class UInventoryComponent;
class UPlayerHUD;
class UInputAction;
class UInputMappingContext;

UCLASS()
class WILDOMISSION_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Jump() override;

private:

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FirstPersonCameraComponent;
	UPROPERTY(VisibleAnywhere)
		UVitalsComponent* VitalsComponent;
	UPROPERTY(VisibleAnywhere)
		UInventoryComponent* InventoryComponent;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UPlayerHUD> PlayerHUDClass;
	UPROPERTY()
		UPlayerHUD* PlayerHUD;
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

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
};
