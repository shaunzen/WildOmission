// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WildOmission/Core/Interfaces/SavableObjectInterface.h"
#include "BuildAnchorComponent.generated.h"

UENUM()
enum EBuildAnchorType
{
	None				UMETA(DisplayName = "None"),
	FoundationAnchor	UMETA(DisplayName = "Foundation Anchor"),
	WallAnchor			UMETA(DisplayName = "Wall Anchor"),
	DoorAnchor			UMETA(DisplayName = "Door Anchor"),
	FloorAnchor			UMETA(DisplayName = "Floor Anchor")
};

class ADeployable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UBuildAnchorComponent : public USceneComponent, public ISavableObjectInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildAnchorComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	TEnumAsByte<EBuildAnchorType> GetType() const;

	ADeployable* GetAttachedDeployable() const;
	void AttachDeployable(ADeployable* DeployableToAttach);
	
	static TArray<UBuildAnchorComponent*> GetAllBuildAnchorsOfTypeFromList(const TArray<UBuildAnchorComponent*>& BuildAnchorList, TEnumAsByte<EBuildAnchorType> TypeToFind);
	static UBuildAnchorComponent* GetClosestBuildAnchorFromList(const TArray<UBuildAnchorComponent*>& BuildAnchors, const FVector& TestPoint);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EBuildAnchorType> Type;

	UPROPERTY(Replicated, SaveGame)
	ADeployable* AttachedDeployable;

};
