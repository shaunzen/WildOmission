// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeOfDayHandler.generated.h"

// WE NEED TO HAVE SOME KIND OF POINTER TO THE DIRECTIONAL LIGHT IN THE LEVEL

class ADirectionalLight;
UCLASS()
class TIMEOFDAY_API ATimeOfDayHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimeOfDayHandler();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool IsDay() const;
	UFUNCTION(BlueprintCallable)
	bool IsNight() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ADirectionalLight* DirectionalLight;
	
	UPROPERTY(VisibleAnywhere) // TODO save this
	int32 DaysPlayed;
	
	UPROPERTY(VisibleAnywhere) // TODO save this
	float ProgressThroughCurrentDayNormalized;

};
