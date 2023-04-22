// Copyright Telephone Studios. All Rights Reserved.


#include "DeployableThatSpins.h"

void ADeployableThatSpins::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MeshComponent->SetRelativeRotation(MeshComponent->GetRelativeRotation() + FRotator(0.0f, 0.0f, 100.0f * DeltaTime));
}