// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuilderComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddBuildingPrivilegeNotificationSignature, bool, HasBuildingPrivilege);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClearBuildingPrivilegeNotificationSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEPLOYABLES_API UBuilderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuilderComponent();

	void OpenToolCupboardMenu(class AToolCupboard* ToolCupboard);
	
	void OpenSignMenu(class ASign* Sign);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Authorize(class AToolCupboard* ToolCupboard);

	UFUNCTION(Server, Reliable)
	void Server_Deauthorize(class AToolCupboard* ToolCupboard);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ClearAllAuthorized(class AToolCupboard* ToolCupboard);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ChangeSignText(class ASign* SignToChange, const TArray<FString>& NewText);

	FOnAddBuildingPrivilegeNotificationSignature OnAddBuildingPrivilegeNotification;
	FOnClearBuildingPrivilegeNotificationSignature OnClearBuildingPrivilegeNotification;

	// Will return if the current owner has authorization at it's current location
	bool HasBuildingPrivilege() const;

	// Will return if the current owner has authorization at the test location
	bool HasBuildingPrivilege(const FVector& LocationToTest) const;

	// Will return true if the current owner is in the range of any tool cupboard
	bool IsBuildRestrictedZone() const;
	
	// Will return true if the test location is in the range of any tool cupboard
	bool IsBuildRestrictedZone(const FVector& LocationToTest) const;

	FString GetOwnerUniqueID() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UToolCupboardWidget> ToolCupboardWidgetClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USignWidget> SignWidgetClass;

	UPROPERTY()
	class UToolCupboardWidget* ToolCupboardWidget;

	UPROPERTY()
	class USignWidget* SignWidget;

	UFUNCTION()
	void OnToolCupboardWidgetTeardown();

	UFUNCTION(Client, Reliable)
	void Client_OpenToolCupboardMenu(class AToolCupboard* ToolCupboard);

	UFUNCTION()
	void OnSignWidgetTeardown();

	UFUNCTION(Client, Reliable)
	void Client_OpenSignWidget(class ASign* Sign);

};
