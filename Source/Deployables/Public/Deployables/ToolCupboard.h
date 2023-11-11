// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployables/Deployable.h"
#include "ToolCupboard.generated.h"

/**
 * 
 */
UCLASS()
class DEPLOYABLES_API AToolCupboard : public ADeployable
{
	GENERATED_BODY()
	
public:
	AToolCupboard();

	void AuthorizePlayer(const FString& PlayerUniqueID);
	bool IsPlayerAuthorized(const FString& PlayerUniqueID) const;
	void ClearAuthorizedPlayers();

	bool IsWithinRange(const FVector& LocationToTest) const;

	static TArray<AToolCupboard*> GetAllToolCupboards();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditDefaultsOnly)
	float Range;
	
	UPROPERTY(SaveGame)
	TArray<FString> AuthorizedPlayers;

};
