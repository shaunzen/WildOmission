// (c) 2023 Telephone Studios, all rights reserved.

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
