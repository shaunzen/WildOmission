// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WildOmission/Core/Interfaces/SavableObjectInterface.h"
#include "Deployable.generated.h"

UCLASS()
class WILDOMISSION_API ADeployable : public AActor, public ISavableObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeployable();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UStaticMesh* GetMesh() const;

	bool CanBePlacedOnGround() const;
	bool CanBePlacedOnFloor() const;
	bool CanBePlacedOnWall() const;
	bool CanBePlacedInDoorway() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
	bool bCanBePlacedOnGround;

	UPROPERTY(EditDefaultsOnly)
	bool bCanBePlacedOnFloor;

	UPROPERTY(EditDefaultsOnly)
	bool bCanBePlacedOnWall;

	UPROPERTY(EditDefaultsOnly)
	bool bCanBePlacedInDoorway;


};
