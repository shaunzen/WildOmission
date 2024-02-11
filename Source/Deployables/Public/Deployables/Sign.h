// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployables/Deployable.h"
#include "Sign.generated.h"

/**
 * 
 */
UCLASS()
class DEPLOYABLES_API ASign : public ADeployable
{
	GENERATED_BODY()
	
public:
	ASign();

private:
	UPROPERTY(VisibleAnywhere)
	class UTextRenderComponent* TextRenderComponent;

};
