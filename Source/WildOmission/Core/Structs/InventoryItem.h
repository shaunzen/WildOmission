// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemStat.h"
#include "InventoryItem.generated.h"

USTRUCT()
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName Name = FName("");

	UPROPERTY(EditAnywhere)
	int32 Quantity = 0;

	UPROPERTY(EditAnywhere)
	uint32 UniqueID = 0;

	UPROPERTY(EditAnywhere)
	TArray<FItemStat> Stats;

	static bool CompareNames(const FInventoryItem& Item, const FName& ItemName);
	static bool CompareID(const FInventoryItem& Item, const uint32& ItemID);

	bool IsZero() const;
	
	void Set(const FInventoryItem& InItem);
	void Set(const FName& InName, const int32& InQuantity);

	void Clear();

	int32 GetStat(const FName& StatName);
	void SetStat(const FName& StatName, const int32& Value);

private:
	void AddStat(const FName& Name, const int32& Value);

};