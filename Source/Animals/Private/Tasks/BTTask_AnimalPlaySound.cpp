// Copyright Telephone Studios. All Rights Reserved.


#include "Tasks/BTTask_AnimalPlaySound.h"
#include "AIController.h"
#include "Animals/Animal.h"
#include "Kismet/GameplayStatics.h"

UBTTask_AnimalPlaySound::UBTTask_AnimalPlaySound()
{
	NodeName = TEXT("Play Animal Call Sound");
}

EBTNodeResult::Type UBTTask_AnimalPlaySound::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (OwnerComp.GetAIOwner() == nullptr || OwnerComp.GetAIOwner()->GetPawn() == nullptr || !OwnerComp.GetAIOwner()->GetPawn()->HasAuthority())
	{
		return EBTNodeResult::Type::Failed;
	}

	AAnimal* OwnerAnimal = Cast<AAnimal>(OwnerComp.GetAIOwner()->GetPawn());
	if (OwnerAnimal == nullptr)
	{
		return EBTNodeResult::Type::Failed;
	}

	OwnerAnimal->PlayCallSound();

	return EBTNodeResult::Type::Succeeded;
}
