// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemStat.h"
#include "InventoryItem.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame)
	FName Name = FName("");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame)
	int32 Quantity = 0;

	UPROPERTY(EditAnywhere, SaveGame)
	uint32 UniqueID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame)
	TArray<FItemStat> Stats;

	static bool CompareNames(const FInventoryItem& Item, const FName& ItemName);
	static bool CompareID(const FInventoryItem& Item, const uint32& ItemID);

	bool IsZero() const;
	
	void Set(const FInventoryItem& InItem);
	void Set(const FName& InName, const int32& InQuantity);

	void Clear();

	int32 GetStat(const FName& StatName) const;
	void SetStat(const FName& StatName, const int32& Value);

private:
	void AddStat(const FName& Name, const int32& Value);

};