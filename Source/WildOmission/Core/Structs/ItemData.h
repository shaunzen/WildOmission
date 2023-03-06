// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
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
	TArray<struct FItemStat> Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AEquipableItem> EquipItemClass;

	FItemData();

	int32 GetStat(const FName& StatName);

};
