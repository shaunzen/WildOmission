// (c) 2023 Telephone Studios, all rights reserved.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "../ActorComponents/VitalsComponent.h"
#include "../ActorComponents/InventoryComponent.h"
#include "../SceneComponents/InteractionComponent.h"
#include "../Actors/WorldItem.h"
#include "../Widgets/PlayerHUD.h"
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
		if (IsLocallyControlled() && PlayerHUDClass != nullptr)
		{
			PlayerHUD = CreateWidget<UPlayerHUD>(PlayerController, PlayerHUDClass);
			PlayerHUD->AddToViewport();
		}
	}
}

// Called when the game ends
void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PlayerHUD != nullptr)
	{
		PlayerHUD->RemoveFromParent();
		PlayerHUD = nullptr;
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerHUD->SetVitals(VitalsComponent);
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

void APlayerCharacter::Interact()
{
	// If we are looking at an interacable item
		// Add item to inventory
		// Remove item from world
		// Hide prompt?
}

void APlayerCharacter::UpdateInteractionPrompt()
{
	FHitResult HitResult;
	if (InteractionComponent->InteractableItemInRange(HitResult))
	{
		if (AWorldItem* WorldItem = Cast<AWorldItem>(HitResult.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("World item cast properly"));
			PlayerHUD->SetInteractionPrompt(FString::Printf(TEXT("Press 'E' to pickup %s"), *WorldItem->GetItemName().ToString()));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Couldn't cast properly into world item"));
			PlayerHUD->SetInteractionPrompt(FString(TEXT("")));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Interactable in range"));
		PlayerHUD->SetInteractionPrompt(FString(TEXT("")));
	}
}