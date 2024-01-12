// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "SavableObjectDefinition.generated.h"

USTRUCT(BlueprintType)
struct SAVABLEOBJECT_API FSavableObjectDefinition : public FTableRowBase
{
	GENERATED_BODY()

	FSavableObjectDefinition();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> Class;

};