// Copyright Telephone Studios. All Rights Reserved.


#include "WeatherHandlerComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UWeatherHandlerComponent::UWeatherHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeatherHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> CloudActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("CloudActor"), CloudActors);
	if (CloudActors.Num() == 0)
	{
		return;
	}

	CloudComponent = CloudActors[0]->FindComponentByClass<UVolumetricCloudComponent>();
	if (CloudComponent == nullptr)
	{
		return;
	}
	
	CloudMaterial = Cast<UMaterialInstance>(CloudComponent->Material.Get());
	if (CloudMaterial == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Found Cloud Material"));
}


// Called every frame
void UWeatherHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

