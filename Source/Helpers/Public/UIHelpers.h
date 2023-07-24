// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UIColor.h"
#include "UIHelpers.generated.h"

UCLASS()
class HELPERS_API UUIHelpers: public UObject
{
	GENERATED_BODY()

public:
	UUIHelpers();

	static FUIColor* GetUIColor(const FName& ColorName);
	static FLinearColor GetHighlightedColor(FUIColor* Color);
	static FLinearColor GetSelectedColor(FUIColor* Color);

};
