// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployable.h"
#include "Blueprint/UserWidget.h"
#include "WildOmission/Core/Interfaces/Interactable.h"
#include "ItemContainerBase.generated.h"

class UInventoryComponent;

UCLASS()
class WILDOMISSION_API AItemContainerBase : public ADeployable, public IInteractable
{
	GENERATED_BODY()

public:
	AItemContainerBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/*Begin Interactable Interface Implementation*/
	virtual void Interact(AActor* Interactor) override;
	virtual FString PromptText() override;
	/*End Interactable Interface Implementation*/

	UFUNCTION(Server, Reliable)
	void Server_UnOccupy();

	UClass* GetWidgetClass() const;
	UInventoryComponent* GetInventoryComponent() const;

protected:
	UPROPERTY(Replicated)
	bool bOccupied;

	UPROPERTY()
	FString ContainerName;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(VisibleAnywhere)
	UInventoryComponent* InventoryComponent;

};
