// Copyright Telephone Studios. All Rights Reserved.


#include "WaterActor.h"
#include "GameFramework/PhysicsVolume.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"

// Sets default values
AWaterActor::AWaterActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetWorldScale3D(FVector(1000.0f, 1000.0f, 50));
	MeshComponent->SetCollisionProfileName(FName("OverlapAll"));
}

// Called when the game starts or when spawned
void AWaterActor::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		return;
	}

	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AWaterActor::OnComponentBeginOverlap);
	MeshComponent->OnComponentEndOverlap.AddDynamic(this, &AWaterActor::OnComponentEndOverlap);
}

// Called every frame
void AWaterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaterActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWildOmissionCharacter* OverlappingCharacter = Cast<AWildOmissionCharacter>(OtherActor);
	if (OverlappingCharacter == nullptr)
	{
		return;
	}

	OverlappingCharacter->StartSwimming();
}

void AWaterActor::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	AWildOmissionCharacter* OverlappingCharacter = Cast<AWildOmissionCharacter>(OtherActor);
	if (OverlappingCharacter == nullptr)
	{
		return;
	}

	OverlappingCharacter->StopSwimming();
}