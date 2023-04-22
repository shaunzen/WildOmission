// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Deployable.h"
#include "WildOmission/Core/Interfaces/Interactable.h"
#include "WildOmission/Core/Interfaces/SavableObjectInterface.h"
#include "ItemContainerBase.generated.h"

class UInventoryComponent;

UCLASS()
class WILDOMISSION_API AItemContainerBase : public AActor, public IDeployable, public ISavableObjectInterface, public IInteractable
{
	GENERATED_BODY()

public:
	AItemContainerBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/*Begin Interactable Interface Implementation*/
	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;
	/*End Interactable Interface Implementation*/

	/*Begin Deployable Interface Implementation*/
	virtual UStaticMesh* GetMesh() const override;
	/*End Deployable Interface Implementation*/

	
	UFUNCTION(Server, Reliable)
	void Server_UnOccupy();

	UClass* GetWidgetClass() const;
	UInventoryComponent* GetInventoryComponent() const;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(VisibleAnywhere)
	UInventoryComponent* InventoryComponent;

	UPROPERTY(Replicated)
	bool bOccupied;

};
