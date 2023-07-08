// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SavableTimeOfDayHandler.h"
#include "TimeOfDayHandler.generated.h"

class ADirectionalLight;
UCLASS()
class TIMEOFDAY_API ATimeOfDayHandler : public AActor, public ISavableTimeOfDayHandler
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimeOfDayHandler();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Begin ISavableTimeOfDayHandler Implementation
	virtual void SetDaysPlayed(int32 InDaysPlayed) override;
	virtual int32 GetDaysPlayed() const override;

	virtual void SetNormalizedProgressThroughDay(float InProgress) override;
	virtual float GetNormalizedProgressThroughDay() const override;
	// End ISavableTimeOfDayHandler Implementation


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
	
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_DaysPlayed)
	int32 DaysPlayed;
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_NormalizedProgressThroughDay)
	float NormalizedProgressThroughDay;

	void CalculateMoonPhase();

	UFUNCTION()
	void OnRep_DaysPlayed();
	UFUNCTION()
	void OnRep_NormalizedProgressThroughDay();

};
