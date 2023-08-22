// Copyright Telephone Studios. All Rights Reserved.


#include "Color/UIColor.h"

FUIColor::FUIColor()
{
	Default = FLinearColor(FVector4f(0.0f, 0.0f, 0.0f, 1.0f));
	HighlightedOffset = 2.0f;
	SelectedOffset = 3.0f;
}