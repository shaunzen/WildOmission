// Copyright Telephone Studios. All Rights Reserved.


#include "Monsters/PookaPooka.h"
#include "UObject/ConstructorHelpers.h"

// TODO would be cool to have a sound effect when they land on the ground, like some kind of thud to make them sound heavy
// TODO also would be cool if it caused nearby players cameras to shake
// TODO they shoot "Pook" at you and the sound is makes sounds like "Pook"

APookaPooka::APookaPooka()
{
	DefaultWalkSpeed = 400.0f;

	static ConstructorHelpers::FObjectFinder<USoundBase> PookaPookaIdle(TEXT("/Game/Monsters/Audio/PookaPooka/PookaPooka_Idle_Cue"));
	if (PookaPookaIdle.Succeeded())
	{
		IdleSound = PookaPookaIdle.Object;
	}
}

void APookaPooka::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority())
	{
		return;
	}

	const float CurrentVelocity = FMath::Abs(this->GetVelocity().Length());
	if (CurrentVelocity < 10.0f)
	{
		return;
	}

	this->Jump();
}
