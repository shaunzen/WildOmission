// (c) 2023 Telephone Studios, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "InventoryComponent.generated.h"


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
	TSubclassOf<class AWorldItem> WorldItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsStackable;

	FItem()
	{
		DisplayName = FName(TEXT("Item"));
		Description = FString(TEXT("This is an Item."));
		Thumbnail = nullptr;
		WorldItem = nullptr;
		IsStackable = true;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddToInventory(FName ItemName, int32 Quantity);
	void RemoveFromInventory();
	void SwapWithInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	TMap<FName, int32> InventoryContent;
	UPROPERTY()
	int32 MaxSize;
};
