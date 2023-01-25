// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WildOmissionGameMode.generated.h"

class ASaveHandler;

UCLASS()
class WILDOMISSION_API AWildOmissionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	UFUNCTION(Exec)
	void LogPlayerInventoryComponents();

	UFUNCTION(BlueprintCallable, Exec)
	void SaveGame();
	
	UFUNCTION()
	void AddPlayerToPending(APlayerController* InPlayerController);

	TArray<APlayerController*> GetAllPlayerControllers();
private:
	UPROPERTY()
	ASaveHandler* SaveHandler;

};
