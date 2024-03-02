// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/Structure.h"
#include "ChunkManager.h"

AStructure::AStructure()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	
	Identifier = NAME_None;
}

bool AStructure::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);

	return AChunkManager::IsActorNetRelevent(this, ViewTarget);
}

FName AStructure::GetIdentifier() const
{
	return Identifier;
}
