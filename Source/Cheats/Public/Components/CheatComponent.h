// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CheatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHEATS_API UCheatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCheatComponent();

	UFUNCTION(BlueprintCallable)
	void OpenCheatMenu();

	UFUNCTION(BlueprintCallable)
	void CloseCheatMenu();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCheatMenuWidget> CheatMenuWidgetClass;

	UPROPERTY()
	class UCheatMenuWidget* CheatMenuWidget;

};
