// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "AmbientSoundProducerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSIONCORE_API UAmbientSoundProducerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAmbientSoundProducerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	USoundBase* DayCue;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* NightCue;

	UFUNCTION()
	void PlaySoundCue();

};
