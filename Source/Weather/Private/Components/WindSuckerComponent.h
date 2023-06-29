// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WindSuckerComponent.generated.h"


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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	TArray<TEnumAsByte<enum EObjectTypeQuery> > ObjectTypesToAffect;
	FCollisionObjectQueryParams CollisionObjectQueryParams;

private:	
	float Radius;
	float ForceStrength;
	TEnumAsByte<enum ERadialImpulseFalloff> Falloff;	

	void AddObjectTypeToAffect(TEnumAsByte<enum EObjectTypeQuery> ObjectType);
	void RemoveObjectTypeToAffect(TEnumAsByte<enum EObjectTypeQuery> ObjectType);
	void AddCollisionChannelToAffect(enum ECollisionChannel CollisionChannel);
	void UpdateCollisionObjectQueryParams();

	bool HasLineOfSightToActor(AActor* InActor) const;

};