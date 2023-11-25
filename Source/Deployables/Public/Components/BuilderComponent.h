// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuilderComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEPLOYABLES_API UBuilderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuilderComponent();

	void OpenToolCupboardMenu(class AToolCupboard* ToolCupboard);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Authorize(class AToolCupboard* ToolCupboard);

	UFUNCTION(Server, Reliable)
	void Server_Deauthorize(class AToolCupboard* ToolCupboard);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ClearAllAuthorized(class AToolCupboard* ToolCupboard);

	// Will return if the current owner has authorization at it's current location
	bool HasAuthorization() const;

	// Will return if the current owner has authorization at the test location
	bool HasAuthorizationAtLocation(const FVector& LocationToTest) const;
	
	FString GetOwnerUniqueID() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UToolCupboardWidget> ToolCupboardWidgetClass;

	UPROPERTY()
	class UToolCupboardWidget* ToolCupboardWidget;

	UFUNCTION()
	void OnToolCupboardWidgetTeardown();

	UFUNCTION(Client, Reliable)
	void Client_OpenToolCupboardMenu(class AToolCupboard* ToolCupboard);

};
