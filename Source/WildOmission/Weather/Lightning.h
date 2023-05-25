// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lightning.generated.h"

UCLASS()
class WILDOMISSION_API ALightning : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightning();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* LightComponent;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* ThunderSound;

};
