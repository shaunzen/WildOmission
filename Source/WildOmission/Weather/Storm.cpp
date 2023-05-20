// Copyright Telephone Studios. All Rights Reserved.


#include "Storm.h"

// Sets default values
AStorm::AStorm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStorm::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStorm::OnSpawn()
{

}

// Called every frame
void AStorm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

