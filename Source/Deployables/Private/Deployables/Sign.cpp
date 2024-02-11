// Copyright Telephone Studios. All Rights Reserved.


#include "Deployables/Sign.h"
#include "Components/TextRenderComponent.h"

ASign::ASign()
{
	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
}

void ASign::Interact(AActor* Interactor)
{
	Super::Interact(Interactor);

	// TODO only allow to bring up if we have building priv
	// TODO bring up control menu

}

FString ASign::PromptText()
{
	Super::PromptText();

	return TEXT("");
}