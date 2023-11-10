// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deployables/ItemContainerBase.h"
#include "StorageBox.generated.h"

/**
 * 
 */
UCLASS()
class DEPLOYABLES_API AStorageBox : public AItemContainerBase
{
	GENERATED_BODY()
	
public:
	AStorageBox();

protected:
	UPROPERTY(VisibleAnywhere)
	class ULockComponent* LockComponent;

	virtual bool CanOpen(AActor* Interactor) const override;

};
