// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "UIColor.generated.h"

USTRUCT(BlueprintType)
struct FUIColor : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Default;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HighlightedOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SelectedOffset;

};