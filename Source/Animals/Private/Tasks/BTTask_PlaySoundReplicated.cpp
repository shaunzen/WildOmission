// Copyright Telephone Studios. All Rights Reserved.


#include "Tasks/BTTask_PlaySoundReplicated.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

UBTTask_PlaySoundReplicated::UBTTask_PlaySoundReplicated()
{
	NodeName = TEXT("Play Sound Replicated");
	Sound = nullptr;
}

EBTNodeResult::Type UBTTask_PlaySoundReplicated::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (OwnerComp.GetAIOwner() == nullptr || !OwnerComp.GetAIOwner()->GetPawn()->HasAuthority())
	{
		return EBTNodeResult::Type::Failed;
	}

	Multi_PlaySound(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation());

	return EBTNodeResult::Type::Succeeded;
}

void UBTTask_PlaySoundReplicated::Multi_PlaySound_Implementation(const FVector& SoundLocation)
{
	if (Sound == nullptr)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, SoundLocation);
}
