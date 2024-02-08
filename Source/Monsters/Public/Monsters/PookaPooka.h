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

	virtual void Tick(float DeltaTime) override;

	virtual void Landed(const FHitResult& Hit) override;

};
