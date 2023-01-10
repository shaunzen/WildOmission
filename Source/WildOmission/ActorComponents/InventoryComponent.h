// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "InventoryComponent.generated.h"

class UInventoryWidget;

USTRUCT(BlueprintType)
struct FItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StackSize;
	
	FItem()
	{
		DisplayName = FName(TEXT("Item"));
		Description = FString(TEXT("This is an Item."));
		Thumbnail = nullptr;
		Mesh = nullptr;
		StackSize = 1000;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	
	void Setup(UInventoryWidget* InInventoryWidget);

	void AddItem(FName ItemName, int32 Quantity);
	void RemoveItem();
	void SwapItem();

	void SpawnWorldItem(FName ItemName, int32 Quantity = 1);

	FItem* GetItemData(FName ItemName);
	int32 GetMaxSize();
	TMap<FName, int32>* GetContent();

	void SetMaxSize(int32 InMaxSize);
	UInventoryWidget* GetWidget();

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, int32> InventoryContent;
	UPROPERTY(EditDefaultsOnly)
	int32 MaxSize;
	UPROPERTY(EditDefaultsOnly)
	UDataTable* ItemDataTable;

	UFUNCTION(Server, Reliable)
	void Server_AddItem(FName ItemName, int32 Quantity);
	
	UFUNCTION(Server, Reliable)
	void Server_SpawnWorldItem(FName ItemName, int32 Quantity);

	UPROPERTY()
	UInventoryWidget* InventoryWidget;

};
