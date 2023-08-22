// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UIColor.h"
#include "UIColors.generated.h"

UCLASS()
class CUSTOMUI_API UUIColors : public UObject
{
	GENERATED_BODY()

public:
	UUIColors();

	static FUIColor* GetBaseColor(const FName& ColorName);
	static FLinearColor GetHighlightedColor(FUIColor* Color);
	static FLinearColor GetSelectedColor(FUIColor* Color);

};
