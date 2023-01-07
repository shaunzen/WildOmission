// Copyright Telephone Studios. All Rights Reserved.


#include "PlayerCharacter.h"
#include "../PlayerControllers/PlayerCharacterController.h"
#include "Components/StaticMeshComponent.h"
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
	
	// Set HUD to nullptr before its created
	PlayerHUDWidget = nullptr;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(RootComponent);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Setup vitals component
	VitalsComponent = CreateDefaultSubobject<UVitalsComponent>(FName("VitalsComponent"));
	
	// Setup inventory component
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(FName("InventoryComponent"));
	InventoryComponent->SetIsReplicated(true);

	// Setup interaction component
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(FName("InteractionComponent"));
	InteractionComponent->SetupAttachment(FirstPersonCameraComponent);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (IsLocallyControlled() && PlayerController && GEngine != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Valid player controller on actor: %s"), *GetActorNameOrLabel());
		//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.0f, FColor::Red, FString("Player controller was valid"));
	}
	if (PlayerController == nullptr)
	{

		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter: Failed to cast Controller to a PlayerController"));
		return;
	}
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter: EISubsystem was nullptr"));
		return;
	}
	Subsystem->AddMappingContext(DefaultMappingContext, 0);

	// Return if we are not being locally controlled
	if (!IsLocallyControlled())
	{
		return;
	}
	
	// Hide player model?
	GetMesh()->SetVisibility(false);

	// Create the player's hud
	if (PlayerHUDWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter: PlayerHUDWidgetClass was nullptr"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Creating Player HUD Widget"));
	PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(PlayerController, PlayerHUDWidgetClass);
	PlayerHUDWidget->AddToViewport();
	
	// Set the player's inventory component to use the player's inventory widget
	InventoryComponent->Setup(PlayerHUDWidget->GetInventoryWidget());
	InteractionComponent->SetHUDWidget(PlayerHUDWidget);
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
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, InteractionComponent, &UInteractionComponent::Interact);
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
	// Return early if the inventory menu is open
	if (PlayerHUDWidget->InventoryOpen())
	{
		return;
	}
	FVector2D LookAxis = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(LookAxis.Y);
}

void APlayerCharacter::ToggleInventory()
{
	if (PlayerHUDWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter: PlayerHUDWidget was nullptr"));
		return;
	}
	PlayerHUDWidget->ToggleInventory();
}

UInventoryComponent* APlayerCharacter::GetInventoryComponent()
{
	return InventoryComponent;
}