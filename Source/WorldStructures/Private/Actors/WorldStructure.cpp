// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/WorldStructure.h"
#include "ChunkManager.h"

AWorldStructure::AWorldStructure()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	
	Identifier = NAME_None;
}

bool AWorldStructure::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);

	return AChunkManager::IsActorNetRelevent(this, ViewTarget);
}

FName AWorldStructure::GetIdentifier() const
{
	return Identifier;
}
