// (c) 2023 Telephone Studios, all rights reserved.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "../ActorComponents/VitalsComponent.h"
#include "../ActorComponents/InventoryComponent.h"
#include "../SceneComponents/InteractionComponent.h"
#include "../Actors/WorldItem.h"
#include "../Widgets/PlayerHUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(RootComponent);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Setup player components
	VitalsComponent = CreateDefaultSubobject<UVitalsComponent>(FName("VitalsComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(FName("InventoryComponent"));
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(FName("InteractionComponent"));
	InteractionComponent->SetupAttachment(FirstPersonCameraComponent);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// Setup input mapping context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		
		// Add HUD to viewport
		if (IsLocallyControlled() && PlayerHUDWidgetClass != nullptr)
		{
			PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(PlayerController, PlayerHUDWidgetClass);
			PlayerHUDWidget->AddToViewport();
		}
	}
}

// Called when the game ends
void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PlayerHUDWidget != nullptr)
	{
		PlayerHUDWidget->RemoveFromParent();
		PlayerHUDWidget = nullptr;
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerHUDWidget == nullptr)
	{
		return;
	}
	PlayerHUDWidget->SetVitals(VitalsComponent);
	UpdateInteractionPrompt();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Cast PlayerInputComponent to UEnhancedPlayerInputComponent
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Bind function callbacks to input actions
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
	EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &APlayerCharacter::ToggleInventory);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MoveAxis = Value.Get<FVector2D>();

	AddMovementInput(FirstPersonCameraComponent->GetForwardVector(), MoveAxis.Y);
	AddMovementInput(FirstPersonCameraComponent->GetRightVector(), MoveAxis.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(LookAxis.Y);
}

//**************************************
// TODO simplfy interaction code

void APlayerCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interaction"));
	FHitResult HitResult;
	// If the actor we are looking at is an interactable
	if (InteractionComponent->InteractableItemInRange(HitResult))
	{
		// If the actor is a world item
		if (AWorldItem* WorldItem = Cast<AWorldItem>(HitResult.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Interaction with world item with name of: %s"), *WorldItem->GetItemName().ToString());
			// Add item to inventory
			InventoryComponent->AddItem(WorldItem->GetItemName(), WorldItem->GetItemQuantity());
			// Remove item from world
			WorldItem->Destroy();
		}
	}
}

void APlayerCharacter::ToggleInventory()
{
	PlayerHUDWidget->ToggleInventory();
}

void APlayerCharacter::UpdateInteractionPrompt()
{
	FHitResult HitResult;
	// If we are looking at an interactable item
	if (InteractionComponent->InteractableItemInRange(HitResult))
	{
		// If the interactable item we are looking at is a world item
		if (AWorldItem* WorldItem = Cast<AWorldItem>(HitResult.GetActor()))
		{
			// Get the item data for the item we are looking at
			FItem* ItemData = InventoryComponent->GetItemData(WorldItem->GetItemName());
			// Return if its nullptr
			if (ItemData == nullptr) return;
			// Set the interaction prompt
			PlayerHUDWidget->SetInteractionPrompt(FString::Printf(TEXT("Press 'E' to pickup %s"), *ItemData->DisplayName.ToString()));
		}
		else
		{
			// Clear the interaction prompt
			PlayerHUDWidget->SetInteractionPrompt(FString(TEXT("")));
		}
	}
	else
	{
		// Clear the interaction prompt
		PlayerHUDWidget->SetInteractionPrompt(FString(TEXT("")));
	}
}