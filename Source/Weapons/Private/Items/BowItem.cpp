// Copyright Telephone Studios. All Rights Reserved.


#include "Items/BowItem.h"

ABowItem::ABowItem()
{
	ProjectileClass = nullptr;
	AtFullDraw = false;
}

void ABowItem::OnSecondaryPressed()
{
	Super::OnSecondaryPressed();
	// todo play draw montage
}

void ABowItem::OnSecondaryAnimationClimax(bool FromFirstPersonInstance)
{
	Super::OnSecondaryAnimationClimax(FromFirstPersonInstance);
	// TODO set fulldraw
}

void ABowItem::OnSecondaryReleased()
{
	Super::OnSecondaryReleased();
	// TODO if fulldraw
}
