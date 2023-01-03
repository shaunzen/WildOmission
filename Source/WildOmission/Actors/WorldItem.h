// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldItem.generated.h"

UCLASS()
class WILDOMISSION_API AWorldItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldItem();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	FName GetItemName();
	UFUNCTION(BlueprintCallable)
	int32 GetItemQuantity();
	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetItemMesh();

	void Collect();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(EditDefaultsOnly)
	FName ItemName;
	UPROPERTY(EditDefaultsOnly)
	int32 ItemQuantity;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ItemMesh;

	// Calls collection on the server
	UFUNCTION(Server, Reliable)
	void Server_Collect(AActor* Item);
};
