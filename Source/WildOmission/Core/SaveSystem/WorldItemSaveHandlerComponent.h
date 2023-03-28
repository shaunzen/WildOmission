// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WorldItemSaveHandlerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UWorldItemSaveHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWorldItemSaveHandlerComponent();
	
	void Save(TArray<struct FWorldItemSave>& OutSaves);

	void Load(const TArray<struct FWorldItemSave>& InSaves);
private:

};
