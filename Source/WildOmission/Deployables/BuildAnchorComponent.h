// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Interfaces/SavableObject.h"
#include "BuildAnchorComponent.generated.h"

UENUM()
enum EBuildAnchorType
{
	None				UMETA(DisplayName = "None"),
	FoundationAnchor	UMETA(DisplayName = "Foundation Anchor"),
	WallAnchor			UMETA(DisplayName = "Wall Anchor"),
	FloorAnchor			UMETA(DisplayName = "Floor Anchor"),
	DoorAnchor			UMETA(DisplayName = "Door Anchor"),
	HouseStairsAnchor	UMETA(DisplayName = "House Stairs Anchor")
};

class ADeployable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UBuildAnchorComponent : public UStaticMeshComponent, public ISavableObject
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildAnchorComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FTransform GetCorrectedTransform() const;

	TEnumAsByte<EBuildAnchorType> GetType() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EBuildAnchorType> Type;

	UPROPERTY(Replicated, ReplicatedUsing = "OnRep_IsOccupied", SaveGame)
	bool IsOccupied;

	UFUNCTION()
	void OnRep_IsOccupied();

	UFUNCTION()
	virtual void OnLoadComplete_Implementation() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

};
