// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Interfaces/SavableObject.h"
#include "LockComponent.generated.h"

class ALock;

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

	void ApplyLock();
	void RemoveLock();

	bool IsLockPlaced() const;

	void SetCode(const FString& NewCode);
	void ClearCode();
	FString GetCode() const;

	void AddAuthorizedPlayer(const FString& PlayerToAuthorize);

	bool IsPlayerAuthorized(const FString& PlayerUniqueID) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_HasLock, SaveGame)
	bool HasLock;

	UPROPERTY(SaveGame)
	TArray<FString> AuthorizedPlayers;

	UPROPERTY(SaveGame)
	FString Code;

	UPROPERTY()
	TSubclassOf<ALock> CodeLockClass;

	UPROPERTY()
	ALock* SpawnedLock;

	UFUNCTION()
	void OnOwnerDestroyed(AActor* DestroyedActor);

	UFUNCTION()
	void OnRep_HasLock();

	UFUNCTION()
	virtual void OnLoadComplete_Implementation() override;

};
