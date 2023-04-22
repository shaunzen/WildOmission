// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"

static UDataTable* ItemDataTable = nullptr;

UWildOmissionStatics::UWildOmissionStatics()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableObject(TEXT("/Game/WildOmission/Core/DataTables/DT_Items"));
	if (ItemDataTableObject.Succeeded())
	{
		ItemDataTable = ItemDataTableObject.Object;
	}
}

FItemData* UWildOmissionStatics::GetItemData(const FName& ItemName)
{
	if (ItemDataTable == nullptr)
	{
		return nullptr;
	}

	static const FString ContextString(TEXT("Item Data Context"));

	return ItemDataTable->FindRow<FItemData>(ItemName, ContextString, true);
}