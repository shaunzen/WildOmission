// Copyright Telephone Studios. All Rights Reserved.


#include "Tornado.h"

// Sets default values
ATornado::ATornado()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));
	
}

// Called when the game starts or when spawned
void ATornado::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MeshComponent->SetRelativeRotation(MeshComponent->GetRelativeRotation() + FRotator(0.0f, 1.0f * DeltaTime, 0.0f));
}

