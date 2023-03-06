// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

//TODO remove these and forward declare
#include "ItemStat.h"
#include "WildOmission/Items/EquipableItem.h"

#include "UObject/NoExportTypes.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemStat> Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEquipableItem> EquipItemClass;


	//TODO move these to source file
	FItemData()
	{
		DisplayName = FName(TEXT("Item"));
		Description = FString(TEXT("This is an Item."));
		Thumbnail = nullptr;
		Mesh = nullptr;
		StackSize = 1000;
		EquipItemClass = nullptr;
	}

	int32 GetStat(const FName& StatName)
	{
		int32 StatValue = -1;

		if (Stats.Num() == 0)
		{
			return StatValue;
		}

		for (const FItemStat& Stat : Stats)
		{
			if (Stat.Name != StatName)
			{
				continue;
			}

			StatValue = Stat.Value;
			break;
		}

		return StatValue;
	}
};
