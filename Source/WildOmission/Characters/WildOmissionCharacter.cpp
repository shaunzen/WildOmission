// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionCharacter.h"
#include "HumanAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "UObject/ConstructorHelpers.h"
#include "WildOmission/Components/EquipComponent.h"
#include "WildOmission/Components/InventoryManipulatorComponent.h"
#include "WildOmission/Components/PlayerInventoryComponent.h"
#include "WildOmission/Deployables/ItemContainerBase.h"
#include "WildOmission/Components/CraftingComponent.h"
#include "WildOmission/Components/InteractionComponent.h"
#include "WildOmission/Components/VitalsComponent.h"
#include "WildOmission/Components/NameTagComponent.h"
#include "WildOmission/Items/EquipableItem.h"
#include "WildOmission/Core/PlayerControllers/WildOmissionPlayerController.h"
#include "WildOmission/UI/Player/PlayerHUDWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

//********************************
// Setup/General Actor Functionality
//********************************
AWildOmissionCharacter::AWildOmissionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<UPlayerHUDWidget> PlayerHUDWidgetBlueprintClass(TEXT("/Game/WildOmission/UI/Player/WBP_PlayerHUD"));
	ConstructorHelpers::FClassFinder<UHumanAnimInstance> PlayerArmsAnimBlueprintClass(TEXT("/Game/WildOmission/Characters/Human/Animation/ABP_Human_FirstPerson"));
	ConstructorHelpers::FClassFinder<UHumanAnimInstance> PlayerThirdPersonAnimBlueprintClass(TEXT("/Game/WildOmission/Characters/Human/Animation/ABP_Human_ThirdPerson"));
	ConstructorHelpers::FClassFinder<AItemContainerBase> PlayerRagdollBlueprint(TEXT("/Game/WildOmission/Characters/Human/BP_Human_Ragdoll"));
	ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContextBlueprint(TEXT("/Game/WildOmission/Core/Input/MC_DefaultMappingContext"));
	ConstructorHelpers::FObjectFinder<UInputAction> MoveActionBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_Move"));
	ConstructorHelpers::FObjectFinder<UInputAction> LookActionBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_Look"));
	ConstructorHelpers::FObjectFinder<UInputAction> SprintActionBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_Sprint"));
	ConstructorHelpers::FObjectFinder<UInputAction> JumpActionBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_Jump"));
	ConstructorHelpers::FObjectFinder<UInputAction> InteractActionBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_Interact"));
	ConstructorHelpers::FObjectFinder<UInputAction> PrimaryActionBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_Primary"));
	ConstructorHelpers::FObjectFinder<UInputAction> SecondaryActionBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_Secondary"));
	ConstructorHelpers::FObjectFinder<UInputAction> ToggleInventoryMenuActionBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_ToggleInventoryMenu"));
	ConstructorHelpers::FObjectFinder<UInputAction> ToggleCraftingMenuActionBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_ToggleCraftingMenu"));
	ConstructorHelpers::FObjectFinder<UInputAction> ToolbarSelectionIncrementBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_ToolbarSelectionIncrement"));
	ConstructorHelpers::FObjectFinder<UInputAction> ToolbarSelectionDecrementBlueprint(TEXT("/Game/WildOmission/Core/Input/InputActions/IA_ToolbarSelectionDecrement"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerArmsMeshObject(TEXT("/Game/WildOmission/Art/Characters/SK_HumanFirstPersonArms"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerThirdPersonMeshObject(TEXT("/Game/WildOmission/Art/Characters/SK_Human"));

	if (PlayerHUDWidgetBlueprintClass.Class == nullptr
		|| PlayerArmsAnimBlueprintClass.Class == nullptr
		|| PlayerThirdPersonAnimBlueprintClass.Class == nullptr
		|| PlayerRagdollBlueprint.Class == nullptr
		|| DefaultMappingContextBlueprint.Object == nullptr
		|| MoveActionBlueprint.Object == nullptr
		|| LookActionBlueprint.Object == nullptr
		|| SprintActionBlueprint.Object == nullptr
		|| JumpActionBlueprint.Object == nullptr
		|| InteractActionBlueprint.Object == nullptr
		|| PrimaryActionBlueprint.Object == nullptr
		|| SecondaryActionBlueprint.Object == nullptr
		|| ToggleInventoryMenuActionBlueprint.Object == nullptr
		|| ToggleCraftingMenuActionBlueprint.Object == nullptr
		|| ToolbarSelectionIncrementBlueprint.Object == nullptr
		|| ToolbarSelectionDecrementBlueprint.Object == nullptr
		|| PlayerArmsMeshObject.Object == nullptr
		|| PlayerThirdPersonMeshObject.Object == nullptr)
	{
		return;
	}

	PlayerHUDWidgetClass = PlayerHUDWidgetBlueprintClass.Class;

	DefaultMappingContext = DefaultMappingContextBlueprint.Object;
	MoveAction = MoveActionBlueprint.Object;
	LookAction = LookActionBlueprint.Object;
	SprintAction = SprintActionBlueprint.Object;
	JumpAction = JumpActionBlueprint.Object;
	InteractAction = InteractActionBlueprint.Object;
	PrimaryAction = PrimaryActionBlueprint.Object;
	SecondaryAction = SecondaryActionBlueprint.Object;
	ToggleInventoryMenuAction = ToggleInventoryMenuActionBlueprint.Object;
	ToggleCraftingMenuAction = ToggleCraftingMenuActionBlueprint.Object;
	ToolbarSelectionIncrementAction = ToolbarSelectionIncrementBlueprint.Object;
	ToolbarSelectionDecrementAction = ToolbarSelectionDecrementBlueprint.Object;

	GetMesh()->SetSkeletalMesh(PlayerThirdPersonMeshObject.Object);
	GetMesh()->SetAnimClass(PlayerThirdPersonAnimBlueprintClass.Class);

	PlayerHUDWidget = nullptr;

	RagdollClass = PlayerRagdollBlueprint.Class;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(RootComponent);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	FirstPersonArmsMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("FirstPersonArmsComponent"));
	FirstPersonArmsMeshComponent->SetupAttachment(FirstPersonCameraComponent);
	FirstPersonArmsMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FirstPersonArmsMeshComponent->SetSkeletalMesh(PlayerArmsMeshObject.Object);
	FirstPersonArmsMeshComponent->SetAnimClass(PlayerArmsAnimBlueprintClass.Class);
	FirstPersonArmsMeshComponent->SetRelativeLocation(FVector(-5.0f, 0.0f, -150.0f));
	FirstPersonArmsMeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	FirstPersonArmsMeshComponent->SetRelativeScale3D(FVector(0.9f, 0.9f, 0.9f));
	FirstPersonArmsMeshComponent->SetVisibility(false);
	FirstPersonArmsMeshComponent->SetCastShadow(false);

	EquipComponent = CreateDefaultSubobject<UEquipComponent>(FName("EquipComponent"));
	EquipComponent->SetupAttachment(FirstPersonCameraComponent);

	VitalsComponent = CreateDefaultSubobject<UVitalsComponent>(FName("VitalsComponent"));
	
	InventoryManipulatorComponent = CreateDefaultSubobject<UInventoryManipulatorComponent>(FName("InventoryManipulatorComponent"));
	
	InventoryComponent = CreateDefaultSubobject<UPlayerInventoryComponent>(FName("InventoryComponent"));
	
	CraftingComponent = CreateDefaultSubobject<UCraftingComponent>(FName("CraftingComponent"));

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(FName("InteractionComponent"));
	InteractionComponent->SetupAttachment(FirstPersonCameraComponent);

	NameTag = CreateDefaultSubobject<UNameTagComponent>(FName("NameTag"));
	NameTag->SetupAttachment(RootComponent);
	NameTag->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	GetCharacterMovement()->JumpZVelocity = 350.0f;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	DesiredMovementSpeed = 300.0f;
}

void AWildOmissionCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWildOmissionCharacter, DesiredMovementSpeed);
}

void AWildOmissionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetupEnhancedInputSubsystem();
	SetupMesh();
	SetupPlayerHUD();
	EndSprint();
}

void AWildOmissionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!HasAuthority())
	{
		return;
	}

	Client_UpdateHeadPitch(GetControlRotation().GetNormalized().Pitch);
	if (GetCharacterMovement()->IsSwimming())
	{
		HandleUnderwater();
	}
}

void AWildOmissionCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	SetupEnhancedInputSubsystem();
	SetupMesh();
	SetupPlayerHUD();
}

void AWildOmissionCharacter::UnPossessed()
{
	Super::UnPossessed();

	EquipComponent->DestroyEquipedItem();
}

void AWildOmissionCharacter::SetupEnhancedInputSubsystem()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController == nullptr)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem == nullptr)
	{
		return;
	}

	Subsystem->AddMappingContext(DefaultMappingContext, 0);
}

void AWildOmissionCharacter::SetupMesh()
{
	GetMesh()->SetVisibility(!IsLocallyControlled());
	FirstPersonArmsMeshComponent->SetVisibility(IsLocallyControlled());
}

void AWildOmissionCharacter::SetupPlayerHUD()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!IsLocallyControlled() || PlayerController == nullptr || PlayerHUDWidget != nullptr || PlayerHUDWidgetClass == nullptr)
	{
		return;
	}

	PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(PlayerController, PlayerHUDWidgetClass);
	PlayerHUDWidget->AddToViewport();
}

void AWildOmissionCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PlayerHUDWidget == nullptr)
	{
		return;
	}
	
	PlayerHUDWidget->RemoveFromParent();
	PlayerHUDWidget = nullptr;
}

void AWildOmissionCharacter::HandleDeath()
{
	AWildOmissionPlayerController* OurController = Cast<AWildOmissionPlayerController>(Controller);
	if (OurController == nullptr)
	{
		return;
	}
	OurController->Client_ShowDeathMenu();

	// Create lootable container with inventory
	// Is Spawning kinda in the air, might want to fix that
	AItemContainerBase* SpawnedRagdoll = GetWorld()->SpawnActor<AItemContainerBase>(RagdollClass, GetActorTransform());
	if (SpawnedRagdoll == nullptr)
	{
		return;
	}

	// Set Items to be this players items

	Destroy();
}

void AWildOmissionCharacter::HandleUnderwater()
{
	FHitResult HitResult;
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector End = Start + FVector(0.0f, 0.0f, 1000.0f);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel3))
	{
		bUnderwater = true;
		HandleDeath();
	}
	else
	{
		bUnderwater = false;
	}
}

//********************************
// Input
//********************************

void AWildOmissionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Cast PlayerInputComponent to UEnhancedPlayerInputComponent
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Bind function callbacks to input actions
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWildOmissionCharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWildOmissionCharacter::Look);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AWildOmissionCharacter::StartSprint);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AWildOmissionCharacter::EndSprint);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AWildOmissionCharacter::Jump);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, InteractionComponent, &UInteractionComponent::Interact);
	EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Started, this, &AWildOmissionCharacter::PrimaryPressed);
	EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Completed, this, &AWildOmissionCharacter::PrimaryReleased);
	EnhancedInputComponent->BindAction(SecondaryAction, ETriggerEvent::Started, this, &AWildOmissionCharacter::SecondaryPressed);
	EnhancedInputComponent->BindAction(SecondaryAction, ETriggerEvent::Completed, this, &AWildOmissionCharacter::SecondaryReleased);
	EnhancedInputComponent->BindAction(ToggleInventoryMenuAction, ETriggerEvent::Started, this, &AWildOmissionCharacter::ToggleInventoryMenu);
	EnhancedInputComponent->BindAction(ToggleCraftingMenuAction, ETriggerEvent::Started, this, &AWildOmissionCharacter::ToggleCraftingMenu);
	EnhancedInputComponent->BindAction(ToolbarSelectionIncrementAction, ETriggerEvent::Started, this, &AWildOmissionCharacter::ToolbarSelectionIncrement);
	EnhancedInputComponent->BindAction(ToolbarSelectionDecrementAction, ETriggerEvent::Started, this, &AWildOmissionCharacter::ToolbarSelectionDecrement);
}

void AWildOmissionCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MoveAxis = Value.Get<FVector2D>();

	if (!GetCharacterMovement()->IsSwimming())
	{
		AddMovementInput(GetActorForwardVector(), MoveAxis.Y);
		AddMovementInput(GetActorRightVector(), MoveAxis.X);
	}
	else
	{
		AddMovementInput(UKismetMathLibrary::GetForwardVector(GetControlRotation()), MoveAxis.Y);
		AddMovementInput(UKismetMathLibrary::GetRightVector(GetControlRotation()), MoveAxis.X);
	}
}

void AWildOmissionCharacter::Look(const FInputActionValue& Value)
{

	if (PlayerHUDWidget == nullptr || PlayerHUDWidget->IsMenuOpen())
	{
		return;
	}

	FVector2D LookAxis = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(LookAxis.Y);
}

void AWildOmissionCharacter::StartSprint()
{
	Server_Sprint(true);
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	bSprinting = true;
}

void AWildOmissionCharacter::EndSprint()
{
	Server_Sprint(false);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	bSprinting = false;
}

void AWildOmissionCharacter::OnRep_MovementSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = DesiredMovementSpeed;
}

void AWildOmissionCharacter::Server_Sprint_Implementation(bool bShouldSprint)
{
	if (bShouldSprint == true)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		bSprinting = true;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		bSprinting = false;
	}
}

void AWildOmissionCharacter::PrimaryPressed()
{
	if (PlayerHUDWidget == nullptr || PlayerHUDWidget->IsMenuOpen() || !EquipComponent->PrimaryEnabled() || GetCharacterMovement()->IsSwimming())
	{
		return;
	}

	EquipComponent->Server_PrimaryPressed();
}

void AWildOmissionCharacter::PrimaryReleased()
{
	if (PlayerHUDWidget == nullptr || PlayerHUDWidget->IsMenuOpen() || !EquipComponent->PrimaryEnabled() || GetCharacterMovement()->IsSwimming())
	{
		return;
	}

	EquipComponent->Server_PrimaryReleased();
}

void AWildOmissionCharacter::SecondaryPressed()
{
	if (PlayerHUDWidget == nullptr || PlayerHUDWidget->IsMenuOpen() || !EquipComponent->SecondaryEnabled() || GetCharacterMovement()->IsSwimming())
	{
		return;
	}

	EquipComponent->Server_SecondaryPressed();
}

void AWildOmissionCharacter::SecondaryReleased()
{
	if (PlayerHUDWidget == nullptr || PlayerHUDWidget->IsMenuOpen() || !EquipComponent->SecondaryEnabled() || GetCharacterMovement()->IsSwimming())
	{
		return;
	}

	EquipComponent->Server_SecondaryReleased();
}

void AWildOmissionCharacter::ToggleInventoryMenu()
{
	if (PlayerHUDWidget == nullptr)
	{
		return;
	}

	PlayerHUDWidget->ToggleInventoryMenu();
}

void AWildOmissionCharacter::ToggleCraftingMenu()
{
	if (PlayerHUDWidget == nullptr)
	{
		return;
	}

	PlayerHUDWidget->ToggleCraftingMenu();
}

void AWildOmissionCharacter::ToolbarSelectionIncrement()
{
	if (PlayerHUDWidget == nullptr || PlayerHUDWidget->IsMenuOpen())
	{
		return;
	}

	InventoryComponent->IncrementToolbarSelection();
}

void AWildOmissionCharacter::ToolbarSelectionDecrement()
{
	if (PlayerHUDWidget == nullptr || PlayerHUDWidget->IsMenuOpen())
	{
		return;
	}

	InventoryComponent->DecrementToolbarSelection();
}

//********************************
// Getters
//********************************


USkeletalMeshComponent* AWildOmissionCharacter::GetArmsMesh() const
{
	return FirstPersonArmsMeshComponent;
}

float AWildOmissionCharacter::GetHeadPitch() const
{
	return HeadPitch;
}

bool AWildOmissionCharacter::IsUnderwater() const
{
	return bUnderwater;
}

UCameraComponent* AWildOmissionCharacter::GetFirstPersonCameraComponent()
{
	return FirstPersonCameraComponent;
}

UPlayerHUDWidget* AWildOmissionCharacter::GetHUDWidget() const
{
	return PlayerHUDWidget;
}

UVitalsComponent* AWildOmissionCharacter::GetVitalsComponent() const
{
	return VitalsComponent;
}

UInventoryManipulatorComponent* AWildOmissionCharacter::GetInventoryManipulatorComponent() const
{
	return InventoryManipulatorComponent;
}

UPlayerInventoryComponent* AWildOmissionCharacter::GetInventoryComponent() const
{
	return InventoryComponent;
}

UCraftingComponent* AWildOmissionCharacter::GetCraftingComponent() const
{
	return CraftingComponent;
}

//********************************
// RPC
//********************************

void AWildOmissionCharacter::Client_OpenContainer_Implementation(AItemContainerBase* Container)
{
	if (Container == nullptr || PlayerHUDWidget == nullptr)
	{
		return;
	}

	PlayerHUDWidget->OpenContainer(Container);
}

void AWildOmissionCharacter::Client_UpdateHeadPitch_Implementation(const float& NewHeadPitch)
{
	HeadPitch = NewHeadPitch;
}