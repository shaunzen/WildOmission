// Copyright Telephone Studios. All Rights Reserved.


#include "Components/BuilderComponent.h"
#include "UI/ToolCupboardWidget.h"
#include "Deployables/ToolCupboard.h"
#include "GameFramework/PlayerState.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UBuilderComponent::UBuilderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ToolCupboardWidgetClass = nullptr;
	ToolCupboardWidget = nullptr;

	static ConstructorHelpers::FClassFinder<UToolCupboardWidget> ToolCupboardWidgetBlueprint(TEXT("/Game/Deployables/UI/WBP_ToolCupboardMenu"));
	if (ToolCupboardWidgetBlueprint.Succeeded())
	{
		ToolCupboardWidgetClass = ToolCupboardWidgetBlueprint.Class;
	}
}

void UBuilderComponent::OpenToolCupboardMenu(AToolCupboard* ToolCupboard)
{
	Client_OpenToolCupboardMenu(ToolCupboard);
}

void UBuilderComponent::Server_Deauthorize_Implementation(AToolCupboard* ToolCupboard)
{
	if (ToolCupboard == nullptr)
	{
		return;
	}

	ToolCupboard->DeauthorizePlayer(GetOwnerUniqueID());
}

bool UBuilderComponent::Server_Authorize_Validate(AToolCupboard* ToolCupboard)
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr || ToolCupboard == nullptr)
	{
		return false;
	}

	if (!ToolCupboard->IsWithinRange(OwnerActor->GetActorLocation()))
	{
		return false;
	}

	return true;
}

void UBuilderComponent::Server_Authorize_Implementation(AToolCupboard* ToolCupboard)
{
	if (ToolCupboard == nullptr)
	{
		return;
	}

	ToolCupboard->AuthorizePlayer(GetOwnerUniqueID());
}

bool UBuilderComponent::Server_ClearAllAuthorized_Validate(AToolCupboard* ToolCupboard)
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr || ToolCupboard == nullptr)
	{
		return false;
	}

	if (!ToolCupboard->IsPlayerAuthorized(GetOwnerUniqueID()) || !ToolCupboard->IsWithinRange(OwnerActor->GetActorLocation()))
	{
		return false;
	}

	return true;
}

void UBuilderComponent::Server_ClearAllAuthorized_Implementation(AToolCupboard* ToolCupboard)
{
	if (ToolCupboard == nullptr)
	{
		return;
	}

	ToolCupboard->ClearAuthorizedPlayers();
}

// Called when the game starts
void UBuilderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UBuilderComponent::HasBuildingPrivilege() const
{
	const AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
	{
		return false;
	}

	return HasBuildingPrivilege(OwnerActor->GetActorLocation());
}

bool UBuilderComponent::HasBuildingPrivilege(const FVector& LocationToTest) const
{
	const AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
	{
		return false;
	}

	const FString& OwnerID = GetOwnerUniqueID();

	const TArray<AToolCupboard*> SpawnedToolCupboards = AToolCupboard::GetAllToolCupboards();
	for (const AToolCupboard* ToolCupboard : SpawnedToolCupboards)
	{
		if (ToolCupboard == nullptr || !ToolCupboard->IsWithinRange(LocationToTest) || ToolCupboard->IsPlayerAuthorized(OwnerID))
		{
			continue;
		}

		return false;
	}

	return true;
}

bool UBuilderComponent::IsBuildRestrictedZone() const
{
	const AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
	{
		return false;
	}

	return IsBuildRestrictedZone(OwnerActor->GetActorLocation());
}

bool UBuilderComponent::IsBuildRestrictedZone(const FVector& LocationToTest) const
{
	const TArray<AToolCupboard*> SpawnedToolCupboards = AToolCupboard::GetAllToolCupboards();
	for (const AToolCupboard* ToolCupboard : SpawnedToolCupboards)
	{
		if (ToolCupboard == nullptr || !ToolCupboard->IsWithinRange(LocationToTest))
		{
			continue;
		}

		return true;
	}

	return false;
}

FString UBuilderComponent::GetOwnerUniqueID() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
	{
		return TEXT("");
	}

	APlayerState* OwnerPlayerState = OwnerPawn->GetPlayerState();
	if (OwnerPlayerState == nullptr)
	{
		return TEXT("");
	}

	return OwnerPlayerState->GetUniqueId().ToString();
}

void UBuilderComponent::OnToolCupboardWidgetTeardown()
{
	ToolCupboardWidget = nullptr;
}

void UBuilderComponent::Client_OpenToolCupboardMenu_Implementation(AToolCupboard* ToolCupboard)
{
	UWorld* World = GetWorld();
	if (World == nullptr || ToolCupboard == nullptr || ToolCupboardWidgetClass == nullptr || ToolCupboardWidget != nullptr)
	{
		return;
	}

	ToolCupboardWidget = CreateWidget<UToolCupboardWidget>(World, ToolCupboardWidgetClass);
	if (ToolCupboardWidget == nullptr)
	{
		return;
	}

	ToolCupboardWidget->Setup(ToolCupboard);
	ToolCupboardWidget->OnTeardown.AddDynamic(this, &UBuilderComponent::OnToolCupboardWidgetTeardown);
}
