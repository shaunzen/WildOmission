// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquipableItem.generated.h"

class AWildOmissionCharacter;

UCLASS()
class WILDOMISSION_API AEquipableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquipableItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the item is equiped into the players hands
	virtual void Equip(AWildOmissionCharacter* InOwnerCharacter);

	// Called before the item is unequiped
	virtual void OnUnequip();

	// Primary function of the item. example(Eat, Shoot, Heal)
	virtual void Primary();

	// Secondary function of the item. example(Nothing, Aim, Heal others)
	virtual void Secondary();

	UStaticMesh* GetMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool IsOwnedByOurLocalPlayer() const;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere)
	USoundBase* EquipSound;

	AWildOmissionCharacter* GetOwnerCharacter() const;

	void HandleAttachment();

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayEquipSound();

};
