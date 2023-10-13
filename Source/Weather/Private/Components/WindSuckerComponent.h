// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WindSuckerComponent.generated.h"

enum EObjectTypeQuery;
enum ERadialImpulseFalloff;
enum ECollisionChannel;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UWindSuckerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWindSuckerComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetRadius(float InRadius);
	void SetForceStrength(float InForceStrength);

	float GetRadius() const;
	float GetForceStrength() const;

	void SetDamagesPawn(bool InShouldDamagePawns);
	bool GetDamagesPawn() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToAffect;
	FCollisionObjectQueryParams CollisionObjectQueryParams;

private:	
	UPROPERTY(EditDefaultsOnly, Category = "Wind")
	float Radius;
	
	UPROPERTY(EditDefaultsOnly, Category = "Wind")
	float ForceStrength;
	
	UPROPERTY(EditDefaultsOnly, Category = "Wind")
	bool DealsDamageToPawns;

	UPROPERTY(EditDefaultsOnly, Category = "Wind")
	TEnumAsByte<ERadialImpulseFalloff> Falloff;	

	bool HasLineOfSightToActor(AActor* InActor) const;

	// Begin Helper Functions
	void AddObjectTypeToAffect(TEnumAsByte<EObjectTypeQuery> ObjectType);
	void RemoveObjectTypeToAffect(TEnumAsByte<EObjectTypeQuery> ObjectType);
	void AddCollisionChannelToAffect(ECollisionChannel CollisionChannel);
	void UpdateCollisionObjectQueryParams();
	// End Helper Functions

};