// Copyright Telephone Studios. All Rights Reserved.


#include "Actors/WorldGenerationHandler.h"

// Sets default values
AWorldGenerationHandler::AWorldGenerationHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorldGenerationHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorldGenerationHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

