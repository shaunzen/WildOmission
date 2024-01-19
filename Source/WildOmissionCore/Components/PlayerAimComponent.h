// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAimComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSIONCORE_API UPlayerAimComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerAimComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetLookSensitivity() const;
	bool IsAiming() const;

	UFUNCTION()
	void StartAiming();

	UFUNCTION()
	void StopAiming();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	float LookSensitivity;

	UPROPERTY()
	bool Aiming;

	void SetCameraFOV(float NewFOV);

	void GetCurrentArmOffset(FVector& OutCurrentLocationOffset, FRotator& OutCurrentRotationOffset) const;
	void SetArmOffset(const FVector& InNewLocationOffset, const FRotator& InNewRotationOffset);

	float GetCurrentCameraFOV() const;
	FVector GetTargetArmLocationOffset() const;
	FRotator GetTargetArmRotationOffset() const;

};
