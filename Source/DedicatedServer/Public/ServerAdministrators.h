// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ServerAdministrators.generated.h"

UCLASS(config=ServerAdministrators)
class DEDICATEDSERVER_API UServerAdministrators : public UObject
{
	GENERATED_BODY()

public:
	UServerAdministrators();

	void OnCreation();
	virtual void BeginDestroy() override;

	static UServerAdministrators* Get();

	bool IsAdministrator(const FString& UniqueId) const;
	bool IsAdministrator(const FUniqueNetId& UniqueId) const;

	TArray<FString> GetAdministrators() const;
	void SetAdministrators(const TArray<FString>& InAdministrators);

private:
	UPROPERTY(Config)
	TArray<FString> Administrators;

};
