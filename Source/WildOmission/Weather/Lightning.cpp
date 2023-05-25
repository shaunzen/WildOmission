// Copyright Telephone Studios. All Rights Reserved.


#include "Lightning.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALightning::ALightning()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LightComponent = CreateDefaultSubobject<UPointLightComponent>(FName("LightComponent"));
}

// Called when the game starts or when spawned
void ALightning::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALightning::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

