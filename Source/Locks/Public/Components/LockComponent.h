// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Interfaces/SavableObject.h"
#include "LockComponent.generated.h"

class ALock;

USTRUCT()
struct FCodeLockSave
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	bool HasLock = false;

	UPROPERTY(SaveGame)
	TArray<FString> AuthorizedPlayers;

	UPROPERTY(SaveGame)
	FString Code;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOCKS_API ULockComponent : public USceneComponent, public ISavableObject
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Serialize(FArchive& Ar) override;

	void ApplyLock();
	void RemoveLock();

	bool IsLockPlaced() const;
	ALock* GetLock() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Replicated, SaveGame)
	bool HasLock;

	UPROPERTY(SaveGame)
	FCodeLockSave CodeLockSave;

	UPROPERTY()
	TSubclassOf<ALock> CodeLockClass;

	UPROPERTY()
	ALock* SpawnedLock;

	UFUNCTION()
	void OnOwnerDestroyed(AActor* DestroyedActor);

	UFUNCTION()
	virtual void OnLoadComplete_Implementation() override;

};
