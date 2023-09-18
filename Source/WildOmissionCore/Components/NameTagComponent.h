// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "NameTagComponent.generated.h"

/**
 * 
 */
UCLASS()
class WILDOMISSIONCORE_API UNameTagComponent : public UTextRenderComponent
{
	GENERATED_BODY()

public:
	UNameTagComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void UpdateRotation();

	void UpdateName();

	void UpdateVisibility();

};
