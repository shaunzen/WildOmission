// Copyright Telephone Studios. All Rights Reserved.


#include "WaterVolume.h"
#include "Components/BrushComponent.h" 
#include "UObject/ConstructorHelpers.h"

AWaterVolume::AWaterVolume()
{
	WaterPlane = CreateDefaultSubobject<UStaticMeshComponent>(FName("WaterPlane"));
	WaterPlane->SetupAttachment(RootComponent);
	WaterPlane->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	WaterPlane->SetWorldScale3D(FVector(2.0f));
	WaterPlane->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WaterPlane->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	bWaterVolume = true;
	GetBrushComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	ConstructorHelpers::FObjectFinder<UStaticMesh> WaterPlaneMesh(TEXT("/Game/WildOmissionCore/Art/Environment/SM_WaterPlane"));
	if (!WaterPlaneMesh.Succeeded())
	{
		return;
	}

	WaterPlane->SetStaticMesh(WaterPlaneMesh.Object);
}