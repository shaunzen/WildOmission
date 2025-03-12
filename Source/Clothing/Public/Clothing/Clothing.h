// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Clothing.generated.h"


// TODO JACKET
UENUM()
enum EClothingType
{
	HAT,
	TORSO,
	LEGGINGS,
	BOOTS
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLOTHING_API UClothing : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UClothing();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TEnumAsByte<EClothingType> GetType() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<EClothingType> Type;

};
