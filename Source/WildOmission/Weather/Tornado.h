// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tornado.generated.h"

//TODO make savable
UCLASS()
class WILDOMISSION_API ATornado : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATornado();

	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

};
