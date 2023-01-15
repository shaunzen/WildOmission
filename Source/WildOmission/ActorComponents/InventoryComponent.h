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

	// Checks for slot and asks the server to add the item to our contents
	void AddItem(FName ItemName, int32 Quantity);
	void RemoveItem(FName ItemName, int32 Quantity, bool bSpawnInWorld = false);
	void SwapItem();

	// Calls an rpc on the server to spawn a world item
	void SpawnWorldItem(FName ItemName, int32 Quantity = 1);

	// Gets the contents map for this inventory
	TMap<FName, int32>* GetContents();

	// Retrives the data about the item id passed in
	FItem* GetItemData(FName ItemName);

	// Gets the widget this inventory is using
	UInventoryWidget* GetWidget();

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, int32> InventoryContents;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* ItemDataTable;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	UFUNCTION(Server, Reliable)
	void Server_AddItem(FName ItemName, int32 Quantity);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RemoveItem(FName ItemName, int32 Quantity);
	
	UFUNCTION(Server, Reliable)
	void Server_SpawnWorldItem(FName ItemName, int32 Quantity);
	
};
