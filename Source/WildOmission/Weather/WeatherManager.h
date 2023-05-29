// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeatherManager.generated.h"

class AStorm;

UCLASS()
class WILDOMISSION_API AWeatherManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeatherManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	float GetNextStormChanceTime() const;
	void SetNextStormChanceTime(float NewTime);

	// Returns the storm that was spawned
	AStorm* SpawnStorm();

	void ClearAllStorms();

	void RemoveStormFromList(AStorm* StormToRemove);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditDefaultsOnly)
	float MinTimeBetweenStorms;
	UPROPERTY(EditDefaultsOnly)
	float MaxTimeBetweenStorms;

	UPROPERTY(EditAnywhere)
	float NextStormChanceTime;

	UPROPERTY()
	TSubclassOf<AStorm> StormClass;

	UPROPERTY(Replicated)
	float GlobalWindStrength;
	
	UPROPERTY(Replicated)
	FVector2D GlobalWindDirection;
	
	UPROPERTY(Replicated)
	float TornadoOnGround;
	
	UPROPERTY(Replicated)
	FVector2D TornadoLocation;
	
	UPROPERTY()
	TArray<AStorm*> SpawnedStorms;

	void HandleWind();
	void GetNewStormChanceTime();
	void TrySpawnStorm();

};
