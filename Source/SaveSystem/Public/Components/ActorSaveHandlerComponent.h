// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WildOmissionSaveGame.h"
#include "ActorSaveHandlerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAVESYSTEM_API UActorSaveHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorSaveHandlerComponent();
	
	UFUNCTION()
	void SaveActors(TArray<FActorSaveData>& OutSaves);
	
	UFUNCTION()
	void LoadActors(const TArray<FActorSaveData>& InSaves, const int32& SaveFileVersion);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
	TArray<FActorComponentSaveData> FindComponentsByClass(const TArray<FActorComponentSaveData>& ComponentSaveList, UClass* Class);
	FActorComponentSaveData FindComponentDataByName(const TArray<FActorComponentSaveData>& ComponentSaveList, const FName& ComponentName, UClass* ComponentClass = nullptr);

	static FName FindSavableObjectIdentifierByClassPointer(const UClass* Class);
	static UClass* FindSavableObjectClassUsingIdentifier(const FName& Identifier);

	void FixSaveCompatibility(AActor* ActorToFix, const int32& OldSaveFileVersion);

};
