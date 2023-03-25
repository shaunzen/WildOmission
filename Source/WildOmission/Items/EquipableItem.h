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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the item is equiped into the players hands
	virtual void Equip(AWildOmissionCharacter* InOwnerCharacter, const FName& InItemName, const int8& InFromSlotIndex, const uint32& InUniqueID);

	// Called before the item is unequiped
	virtual void OnUnequip();

	// Primary function of the item. example(Eat, Shoot, Heal)
	virtual void Primary();

	// Secondary function of the item. example(Nothing, Aim, Heal others)
	virtual void Secondary();

	UStaticMesh* GetMesh();
	
	void SetLocalVisibility(bool bVisible);

	int8 GetFromSlotIndex() const;

	uint32 GetUniqueItemID() const;

	UAnimSequence* GetEquipPose() const;

	bool IsTwoHanded() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere)
	USoundBase* EquipSound;
	
	UPROPERTY(EditDefaultsOnly)
	UAnimSequence* EquipPose;

	UPROPERTY(EditDefaultsOnly)
	bool bRequiresTwoHands;

	UPROPERTY(Replicated)
	FName ItemName;

	UPROPERTY(Replicated)
	int8 FromSlotIndex;

	UPROPERTY(Replicated)
	uint32 UniqueID;

	AWildOmissionCharacter* GetOwnerCharacter() const;

	UFUNCTION(NetMulticast, Reliable)
	void Client_PlayEquipSound();

};
