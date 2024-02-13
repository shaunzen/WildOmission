// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/Sign.h"
#include "Components/TextRenderComponent.h"
#include "Components/BuilderComponent.h"
#include "Net/UnrealNetwork.h"

// TODO net relevancy
ASign::ASign()
{
	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	TextRenderComponent->SetupAttachment(MeshComponent);

	Text.SetNum(4);
}

void ASign::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASign, Text);
}

void ASign::OnSpawn()
{
	Super::OnSpawn();

	if (TextRenderComponent == nullptr)
	{
		return;
	}
	
	TextRenderComponent->SetText(FText());
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

void ASign::SetText(const TArray<FString>& NewText)
{
	Text = NewText;
	FlushNetDormancy();
	OnRep_Text();
}

TArray<FString> ASign::GetText() const
{
	return Text;
}

void ASign::OnRep_Text()
{
	FString SignString;
	for (int32 i = 0; i < Text.Num(); ++i)
	{
		SignString.Append(Text[i]);
		
		if (i == Text.Num())
		{
			break;
		}

		SignString.Append(LINE_TERMINATOR);
	}

	TextRenderComponent->SetText(FText::FromString(SignString));
}

void ASign::OnLoadComplete_Implementation()
{
	OnRep_Text();
}
