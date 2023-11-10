// Copyright Telephone Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LockOperation.generated.h"

UENUM()
enum ELockOperation
{
	ELO_SetCode,
	ELO_Authorize,
	ELO_ModifyCode
};