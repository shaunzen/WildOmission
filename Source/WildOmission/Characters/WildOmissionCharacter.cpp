// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UObject/ConstructorHelpers.h"
#include "InputMappingContext.h"
#include "WildOmission/Components/InventoryComponent.h"
#include "WildOmission/Components/VitalsComponent.h"
#include "WildOmission/Components/InteractionComponent.h"
#include "WildOmission/UI/Player/PlayerHUDWidget.h"


// Sets default values
AWildOmissionCharacter::AWildOmissionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<UPlayerHUDWidget> PlayerHUDWidgetBlueprintClass(TEXT("/Game/WildOmission/UI/Player/WBP_PlayerHUD"));
	ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContextBlueprint(TEXT("/Game/WildOmission/Core/Input/MC_DefaultMappingContext"));
	ConstructorHelpers::FObjectFinder<UInputAction> MoveActionBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_Move"));
	ConstructorHelpers::FObjectFinder<UInputAction> LookActionBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_Look"));
	ConstructorHelpers::FObjectFinder<UInputAction> JumpActionBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_Jump"));
	ConstructorHelpers::FObjectFinder<UInputAction> InteractActionBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_Interact"));
	ConstructorHelpers::FObjectFinder<UInputAction> PrimaryActionBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_Primary"));
	ConstructorHelpers::FObjectFinder<UInputAction> SecondaryActionBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_Secondary"));
	ConstructorHelpers::FObjectFinder<UInputAction> InventoryActionBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_Inventory"));
	ConstructorHelpers::FObjectFinder<UInputAction> ToolbarSelectionIncrementBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_ToolbarSelectionIncrement"));
	ConstructorHelpers::FObjectFinder<UInputAction> ToolbarSelectionDecrementBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_ToolbarSelectionDecrement"));

	if (PlayerHUDWidgetBlueprintClass.Class == nullptr
		|| DefaultMappingContextBlueprint.Object == nullptr
		|| MoveActionBlueprint.Object == nullptr
		|| LookActionBlueprint.Object == nullptr
		|| JumpActionBlueprint.Object == nullptr
		|| InteractActionBlueprint.Object == nullptr
		|| PrimaryActionBlueprint.Object == nullptr
		|| SecondaryActionBlueprint.Object == nullptr
		|| InventoryActionBlueprint.Object == nullptr
		|| ToolbarSelectionIncrementBlueprint.Object == nullptr
		|| ToolbarSelectionDecrementBlueprint.Object == nullptr)
	{
		return;
	}

	PlayerHUDWidgetClass = PlayerHUDWidgetBlueprintClass.Class;

	DefaultMappingContext = DefaultMappingContextBlueprint.Object;
	MoveAction = MoveActionBlueprint.Object;
	LookAction = LookActionBlueprint.Object;
	JumpAction = JumpActionBlueprint.Object;
	InteractAction = InteractActionBlueprint.Object;
	PrimaryAction = PrimaryActionBlueprint.Object;
	SecondaryAction = SecondaryActionBlueprint.Object;
	InventoryAction = InventoryActionBlueprint.Object;
	ToolbarSelectionIncrementAction = ToolbarSelectionIncrementBlueprint.Object;
	ToolbarSelectionDecrementAction = ToolbarSelectionDecrementBlueprint.Object;

	PlayerHUDWidget = nullptr;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(RootComponent);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	EquipMountPoint = CreateDefaultSubobject<USceneComponent>(FName("EquipMountPoint"));
	EquipMountPoint->SetupAttachment(FirstPersonCameraComponent);

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
void AWildOmissionCharacter::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
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
	InteractionComponent->Setup(this, PlayerHUDWidget);
}

// Called when the game ends
void AWildOmissionCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PlayerHUDWidget != nullptr)
	{
		PlayerHUDWidget->RemoveFromParent();
		PlayerHUDWidget = nullptr;
	}
}

// Called every frame
void AWildOmissionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (PlayerHUDWidget == nullptr)
	{
		return;
	}
	PlayerHUDWidget->SetVitals(VitalsComponent);
}

// Called to bind functionality to input
void AWildOmissionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Cast PlayerInputComponent to UEnhancedPlayerInputComponent
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Bind function callbacks to input actions
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWildOmissionCharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWildOmissionCharacter::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AWildOmissionCharacter::Jump);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, InteractionComponent, &UInteractionComponent::Interact);
	EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Started, this, &AWildOmissionCharacter::Primary);
	EnhancedInputComponent->BindAction(SecondaryAction, ETriggerEvent::Started, this, &AWildOmissionCharacter::Secondary);
	EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AWildOmissionCharacter::ToggleInventory);
	EnhancedInputComponent->BindAction(ToolbarSelectionIncrementAction, ETriggerEvent::Started, this, &AWildOmissionCharacter::ToolbarSelectionIncrement);
	EnhancedInputComponent->BindAction(ToolbarSelectionDecrementAction, ETriggerEvent::Started, this, &AWildOmissionCharacter::ToolbarSelectionDecrement);
}

void AWildOmissionCharacter::EquipItem(TSubclassOf<AEquipableItem> Item)
{
	EquipedItem = GetWorld()->SpawnActor<AEquipableItem>(Item, EquipMountPoint->GetComponentLocation(), EquipMountPoint->GetComponentRotation());
	
	EquipedItem->AttachToComponent(EquipMountPoint, FAttachmentTransformRules::KeepWorldTransform);

	EquipedItem->Equip(this);
}

void AWildOmissionCharacter::Disarm()
{
	if (EquipedItem == nullptr)
	{
		return;
	}

	EquipedItem->Destroy();
	EquipedItem = nullptr;
}

AEquipableItem* AWildOmissionCharacter::GetEquipedItem()
{
	return EquipedItem;
}

void AWildOmissionCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MoveAxis = Value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), MoveAxis.Y);
	AddMovementInput(GetActorRightVector(), MoveAxis.X);
}

void AWildOmissionCharacter::Look(const FInputActionValue& Value)
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

void AWildOmissionCharacter::Primary()
{
	Server_Primary();
}

void AWildOmissionCharacter::Secondary()
{
	Server_Secondary();
}

void AWildOmissionCharacter::ToggleInventory()
{
	if (PlayerHUDWidget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter: PlayerHUDWidget was nullptr"));
		return;
	}
	PlayerHUDWidget->ToggleInventory();
}

void AWildOmissionCharacter::ToolbarSelectionIncrement()
{
	if (InventoryComponent == nullptr)
	{
		return;
	}

	InventoryComponent->IncrementToolbarSelection();
}

void AWildOmissionCharacter::ToolbarSelectionDecrement()
{
	if (InventoryComponent == nullptr)
	{
		return;
	}

	InventoryComponent->DecrementToolbarSelection();
}

UVitalsComponent* AWildOmissionCharacter::GetVitalsComponent()
{
	return VitalsComponent;
}

UInventoryComponent* AWildOmissionCharacter::GetInventoryComponent()
{
	return InventoryComponent;
}

void AWildOmissionCharacter::Server_Primary_Implementation()
{
	if (EquipedItem == nullptr)
	{
		return;
	}

	EquipedItem->Primary();
}

void AWildOmissionCharacter::Server_Secondary_Implementation()
{
	if (EquipedItem == nullptr)
	{
		return;
	}

	EquipedItem->Secondary();
}