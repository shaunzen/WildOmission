// Copyright Telephone Studios. All Rights Reserved.


#include "PlayerSaveHandler.h"

// Sets default values
APlayerSaveHandler::APlayerSaveHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerSaveHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerSaveHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}