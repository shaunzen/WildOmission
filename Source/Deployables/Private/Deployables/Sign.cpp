// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/Sign.h"
#include "Components/TextRenderComponent.h"
#include "Components/BuilderComponent.h"
#include "Net/UnrealNetwork.h"

ASign::ASign()
{
	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	TextRenderComponent->SetupAttachment(MeshComponent);
}

void ASign::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASign, Text);
}

void ASign::Interact(AActor* Interactor)
{
	if (Interactor == nullptr)
	{
		return;
	}

	UBuilderComponent* InteractorBuilderComponent = Interactor->FindComponentByClass<UBuilderComponent>();
	if (InteractorBuilderComponent == nullptr)
	{
		return;
	}

	if (!InteractorBuilderComponent->HasBuildingPrivilege(this->GetActorLocation()))
	{
		return;
	}

	InteractorBuilderComponent->OpenSignMenu(this);
}

FString ASign::PromptText()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return TEXT("");
	}

	APlayerController* LocalPlayerController = World->GetFirstPlayerController();
	if (LocalPlayerController == nullptr)
	{
		return TEXT("");
	}

	APawn* LocalPawn = LocalPlayerController->GetPawn();
	if (LocalPawn == nullptr)
	{
		return TEXT("");
	}

	UBuilderComponent* LocalBuilderComponent = LocalPawn->FindComponentByClass<UBuilderComponent>();
	if (LocalBuilderComponent == nullptr)
	{
		return TEXT("");
	}

	if (!LocalBuilderComponent->HasBuildingPrivilege(this->GetActorLocation()))
	{
		return TEXT("NOPRESSPROMPT_Modification Not Allowed");
	}

	return TEXT("Modify Text");
}

void ASign::SetText(const FString& NewText)
{
	Text = NewText;
	OnRep_Text();
}

FString ASign::GetText() const
{
	return Text;
}

void ASign::OnRep_Text()
{
	TextRenderComponent->SetText(FText::FromString(Text));
}

void ASign::OnLoadComplete_Implementation()
{
	OnRep_Text();
}
