// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemStat.h"
#include "UObject/NoExportTypes.h"
#include "ItemData.generated.h"

UENUM()
enum EItemCategory
{
	Item,
	Resource,
	Component,
	Tool,
	Building,
	Weapon,
	Ammo,
	Medical,
	Food,
	Clothing,
	Misc,
	All
};

class AEquipableItem;

USTRUCT(BlueprintType)
struct INVENTORY_API FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EItemCategory> Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemStat> Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> EquipItemClass;

	FItemData();

	int32 GetStat(const FName& StatName);

};
