// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/Sign.h"
#include "Components/TextRenderComponent.h"

ASign::ASign()
{
	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	TextRenderComponent->SetupAttachment(MeshComponent);
}

void ASign::Interact(AActor* Interactor)
{
	// TODO only allow to bring up if we have building priv
	// TODO bring up control menu

}

FString ASign::PromptText()
{
	return TEXT("");
}