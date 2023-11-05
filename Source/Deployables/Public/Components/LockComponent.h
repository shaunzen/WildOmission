// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Interfaces/SavableObject.h"
#include "LockComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEPLOYABLES_API ULockComponent : public USceneComponent
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

	void SetCode(const FString& NewCode);
	void ClearCode();
	FString GetCode() const;

	void AddAuthorizedPlayer(const FString& PlayerToAuthorize);

	bool IsPlayerAuthorized(const FString& PlayerUniqueID) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* LockMesh;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_HasLock, SaveGame)
	bool HasLock;

	UPROPERTY(SaveGame)
	TArray<FString> AuthorizedPlayers;

	UPROPERTY(SaveGame)
	FString Code;

	// TODO SpawnedLock

	UFUNCTION()
	void OnRep_HasLock();

};
