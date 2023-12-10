// Copyright Telephone Studios. All Rights Reserved.


#include "Structs/ActorSaveData.h"

FActorComponentSaveData::FActorComponentSaveData()
{
	Name = NAME_None;
	Class = nullptr;
}

FActorSaveData::FActorSaveData()
{
	Identifier = NAME_None;
	Transform = FTransform::Identity;
}
