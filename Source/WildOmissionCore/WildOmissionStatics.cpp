// Copyright Telephone Studios. All Rights Reserved.


#include "WildOmissionStatics.h"
#include "WildOmissionCore/GameModes/WildOmissionGameMode.h"

FVector UWildOmissionStatics::GetHostLocationInWorld(UWorld* WorldContextObject)
{
	APlayerController* HostPlayerController = WorldContextObject->GetFirstPlayerController();
	if (HostPlayerController == nullptr)
	{
		return FVector::ZeroVector;
	}

	APawn* HostPawn = HostPlayerController->GetPawn();
	if (HostPawn == nullptr)
	{
		return FVector::ZeroVector;
	}

	return HostPawn->GetActorLocation();
}

float UWildOmissionStatics::GetSwoopLerp(const float& A, const float& B, const float& TransitionHardness, const float& Alpha, const float& StartAlpha, const float& EndAlpha)
{
	float ZeroAlpha = Alpha - StartAlpha;
	float Duration = EndAlpha - StartAlpha;

	return FMath::Lerp(A, B, FMath::Clamp(FMath::Sin((ZeroAlpha / Duration) * 6.0f) * TransitionHardness, 0.0f, 1.0f));
}

void UWildOmissionStatics::GetWorldSize(UWorld* WorldContextObject, FVector2D& OutWorldSize)
{
	if (WorldContextObject == nullptr)
	{
		return;
	}
	AWildOmissionGameMode* GameMode = Cast<AWildOmissionGameMode>(WorldContextObject->GetAuthGameMode());
	if (GameMode == nullptr)
	{
		return;
	}
	ASaveHandler* SaveHandler = GameMode->GetSaveHandler();
	if (SaveHandler == nullptr)
	{
		return;
	}

	OutWorldSize = FVector2D::ZeroVector;
}