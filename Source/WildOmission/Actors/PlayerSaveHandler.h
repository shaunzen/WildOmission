// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerSaveHandler.generated.h"

class ASaveHandler;

UCLASS()
class WILDOMISSION_API APlayerSaveHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerSaveHandler();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// LoadPlayer(APlayerController* PlayerController);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

};
