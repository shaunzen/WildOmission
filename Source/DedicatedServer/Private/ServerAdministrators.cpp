// Copyright Telephone Studios. All Rights Reserved.


#include "ServerAdministrators.h"

static UServerAdministrators* Instance = nullptr;

UServerAdministrators::UServerAdministrators()
{
	Administrators = TArray<FString>
	{
		TEXT("ADMIN_STEAM_ID_HERE")
	};

	LoadConfig();
}

void UServerAdministrators::OnCreation()
{
	Instance = this;
}

void UServerAdministrators::BeginDestroy()
{
	Super::BeginDestroy();

	SaveConfig();
	
	Instance = nullptr;
}

UServerAdministrators* UServerAdministrators::Get()
{
	return Instance;
}

bool UServerAdministrators::IsAdministrator(const FString& UniqueId) const
{
	for (const FString& Administrator : Administrators)
	{
		if (Administrator != UniqueId)
		{
			continue;
		}

		return true;
	}

	return false;
}

bool UServerAdministrators::IsAdministrator(const FUniqueNetId& UniqueId) const
{
	return IsAdministrator(UniqueId.ToString());
}

TArray<FString> UServerAdministrators::GetAdministrators() const
{
	return Administrators;
}

void UServerAdministrators::SetAdministrators(const TArray<FString>& InAdministrators)
{
	Administrators = InAdministrators;
}
