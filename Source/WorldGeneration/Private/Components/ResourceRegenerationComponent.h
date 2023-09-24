// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceRegenerationComponent.generated.h"

struct FWorldGenerationSettings;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UResourceRegenerationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UResourceRegenerationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void CheckNodeRegenerationConditions();

	UPROPERTY(EditDefaultsOnly, Category = "Generation")
	float InnerRegenerationRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Generation")
	float OuterRegenerationRadius;

	UFUNCTION()
	void RegenerateNodesAroundOrigin(const FWorldGenerationSettings& GenerationSettings, const FVector& Origin);

	bool IsTransformWithinInnerRangeOfAnyPlayer(const FTransform& TransformToCheck) const;
	
	TArray<AActor*> FilterActorsByRange(const TArray<AActor*>& InList, const FVector& Origin, float Range);
	
};
