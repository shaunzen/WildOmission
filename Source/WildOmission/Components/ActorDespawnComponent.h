// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WildOmission/Core/Interfaces/SavableObjectInterface.h"
#include "ActorDespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UActorDespawnComponent : public UActorComponent, public ISavableObjectInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorDespawnComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetTimeRemaining() const;
	void SetDespawnTime(float TimeInSeconds);

private:	
	UPROPERTY(EditDefaultsOnly, SaveGame)
	float DespawnTimeSeconds;

};
