// Copyright Telephone Studios. All Rights Reserved.


#include "UIHelpers.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"

static UDataTable* UIColorsTable = nullptr;

UUIHelpers::UUIHelpers()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> UIColorsTableBlueprint(TEXT("/Game/Helpers/DataTables/DT_UIColors"));
	if (UIColorsTableBlueprint.Succeeded())
	{
		UIColorsTable = UIColorsTableBlueprint.Object;
	}
}

FUIColor* UUIHelpers::GetUIColor(const FName& ColorName)
{
	if (UIColorsTable == nullptr)
	{
		return nullptr;
	}

	static const FString ContextString(TEXT("UI Color Context"));

	return UIColorsTable->FindRow<FUIColor>(ColorName, ContextString, true);
}

FLinearColor UUIHelpers::GetHighlightedColor(FUIColor* Color)
{
	return FLinearColor(Color->Default.R + Color->HighlightedOffset, Color->Default.G + Color->HighlightedOffset, Color->Default.B + Color->HighlightedOffset, 1.0f);
}

FLinearColor UUIHelpers::GetSelectedColor(FUIColor* Color)
{
	return FLinearColor(Color->Default.R + Color->SelectedOffset, Color->Default.G + Color->SelectedOffset, Color->Default.B + Color->SelectedOffset, 1.0f);
}
