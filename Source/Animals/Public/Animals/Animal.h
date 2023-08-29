// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/WildOmissionAICharacter.h"
#include "Animal.generated.h"

UCLASS()
class ANIMALS_API AAnimal : public AWildOmissionAICharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAnimal();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
