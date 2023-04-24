// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UBuildAnchorComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildAnchorComponent();
	
	TEnumAsByte<EBuildAnchorType> GetType() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EBuildAnchorType> Type;
};
