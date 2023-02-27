// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EquipComponent.generated.h"

class AEquipableItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WILDOMISSION_API UEquipComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void EquipItem(TSubclassOf<AEquipableItem> Item);
	void Disarm();

	UFUNCTION(BlueprintCallable)
	AEquipableItem* GetEquipedItem();

	UFUNCTION(BlueprintCallable)
	bool IsItemEquiped() const;

	UFUNCTION(Server, Reliable)
	void Server_Primary();

	UFUNCTION(Server, Reliable)
	void Server_Secondary();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* FirstPersonEquipedItem;

	UPROPERTY(Replicated)
	AEquipableItem* EquipedItem;

};
