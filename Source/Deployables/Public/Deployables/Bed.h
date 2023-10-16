// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployables/Deployable.h"
#include "Bed.generated.h"

/**
 * 
 */
UCLASS()
class DEPLOYABLES_API ABed : public ADeployable
{
	GENERATED_BODY()
	
public:
	ABed();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	int32 GetUniqueID() const;

private:
	UPROPERTY(VisibleAnywhere, SaveGame, Replicated)
	int32 UniqueID;

};
