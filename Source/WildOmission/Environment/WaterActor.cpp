// Copyright Telephone Studios. All Rights Reserved.


#include "WaterActor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "WildOmission/Characters/WildOmissionCharacter.h"

// Sets default values
AWaterActor::AWaterActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(FName("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(50.0f), false);
	BoxCollision->SetWorldScale3D(FVector(1000.0f, 1000.0f, 50.0f));
	BoxCollision->SetRelativeLocation(FVector(0.0f, 0.0f, -2245.0f));
	BoxCollision->SetCollisionProfileName(FName("OverlapAll"));
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
	RootComponent = BoxCollision;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));
	MeshComponent->SetupAttachment(BoxCollision);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetWorldScale3D(FVector(1000.0f, 1000.0f, 0.0f));
}

// Called when the game starts or when spawned
void AWaterActor::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AWaterActor::OnComponentBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AWaterActor::OnComponentEndOverlap);
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