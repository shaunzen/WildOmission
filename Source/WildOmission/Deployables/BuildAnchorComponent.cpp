// Copyright Telephone Studios. All Rights Reserved.


#include "BuildAnchorComponent.h"
#include "Deployable.h"

// Sets default values for this component's properties
UBuildAnchorComponent::UBuildAnchorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Type = EBuildAnchorType::FoundationAnchor;
	
	SetRelativeScale3D(FVector(0.9f));
	SetCollisionProfileName(FName("OverlapAllDynamic"));
	CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	ComponentTags.Add("BuildAnchor");
}

// Called when the game starts
void UBuildAnchorComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UBuildAnchorComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UBuildAnchorComponent::OnEndOverlap);
}

FTransform UBuildAnchorComponent::GetCorrectedTransform() const
{
	FTransform CorrectedTransform;
	CorrectedTransform.SetLocation(GetComponentLocation());
	CorrectedTransform.SetRotation(GetComponentRotation().Quaternion());

	return CorrectedTransform;
}

TEnumAsByte<EBuildAnchorType> UBuildAnchorComponent::GetType() const
{
	return Type;
}

void UBuildAnchorComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADeployable* DeployableInPlace = Cast<ADeployable>(OtherActor);
	if (DeployableInPlace == nullptr || OtherComponent->GetClass() == UBuildAnchorComponent::StaticClass() || DeployableInPlace->CanSpawnOnBuildAnchor() != this->Type)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Disabling this build anchor."));
	SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECR_Ignore);
}

void UBuildAnchorComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	ADeployable* DeployableInPlace = Cast<ADeployable>(OtherActor);
	if (DeployableInPlace == nullptr || OtherComponent->GetClass() == UBuildAnchorComponent::StaticClass() || DeployableInPlace->CanSpawnOnBuildAnchor() != this->Type)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Disabling this build anchor."));
	SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECR_Block);
}