// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WildOmissionSaveGame.h"
#include "ActorSaveHandlerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UActorSaveHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorSaveHandlerComponent();
	
	UFUNCTION()
	void SaveActors(TArray<FActorSaveData>& OutSaves);
	
	UFUNCTION()
	void LoadActors(const TArray<FActorSaveData>& InSaves);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
};
