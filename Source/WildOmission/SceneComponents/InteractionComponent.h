// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool InteractableItemInRange(FHitResult& OutHitResult) const;
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	float InteractionRange;
};
