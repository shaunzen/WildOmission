// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Monsters/Monster.h"
#include "PookaPooka.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERS_API APookaPooka : public AMonster
{
	GENERATED_BODY()

public:
	APookaPooka();

	void SetIsChasing(bool IsChasing);

	virtual void Tick(float DeltaTime) override;

	virtual void Landed(const FHitResult& Hit) override;

protected:
	virtual void HandleDeath() override;

private:
	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayLandSound();

};
