// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WildOmission/Core/Interfaces/Interactable.h"
#include "StorageCrate.generated.h"

class UInventoryComponent;

UCLASS()
class WILDOMISSION_API AStorageCrate : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStorageCrate();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*Begin Interactable Interface Implementation*/
	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;
	/*End Interactable Interface Implementation*/

	UClass* GetWidgetClass() const;
	UInventoryComponent* GetInventoryComponent() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Container")
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(VisibleAnywhere)
	UInventoryComponent* InventoryComponent;

};
