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
	
	SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel9);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Ignore);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
	
	CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	bHiddenInGame = true;

	ComponentTags.Add("BuildAnchor");
}

// Called when the game starts
void UBuildAnchorComponent::BeginPlay()
{
	Super::BeginPlay();

	switch (Type)
	{
	case None:
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Block);
		break;
	case FoundationAnchor:
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Block);
		break;
	case WallAnchor:
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6, ECollisionResponse::ECR_Block);
		break;
	case FloorAnchor:
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel7, ECollisionResponse::ECR_Block);
		break;
	case DoorAnchor:
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel8, ECollisionResponse::ECR_Block);
		break;
	}

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
	if (DeployableInPlace && DeployableInPlace->CanSpawnOnBuildAnchor() == this->Type)
	{
		UE_LOG(LogTemp, Warning, TEXT("Deployed a %s in this build anchor"), *DeployableInPlace->GetActorNameOrLabel());
	}

	if (DeployableInPlace == nullptr || DeployableInPlace->CanSpawnOnBuildAnchor() != this->Type)
	{
		return;
	}

	switch (Type)
	{
	case None:
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Ignore);
		break;
	case FoundationAnchor:
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore);
		break;
	case WallAnchor:
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6, ECollisionResponse::ECR_Ignore);
		break;
	case FloorAnchor:
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel7, ECollisionResponse::ECR_Ignore);
		break;
	case DoorAnchor:
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel8, ECollisionResponse::ECR_Ignore);
		break;
	}
}

void UBuildAnchorComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	ADeployable* DeployableInPlace = Cast<ADeployable>(OtherActor);
	if (DeployableInPlace == nullptr || DeployableInPlace->CanSpawnOnBuildAnchor() != this->Type)
	{
		return;
	}
	
	switch (Type)
	{
	case None:
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Block);
		break;
	case FoundationAnchor:
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Block);
		break;
	case WallAnchor:
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6, ECollisionResponse::ECR_Block);
		break;
	case FloorAnchor:
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel7, ECollisionResponse::ECR_Block);
		break;
	case DoorAnchor:
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel8, ECollisionResponse::ECR_Block);
		break;
	}
}