// Copyright Telephone Studios. All Rights Reserved.


#include "Items/ManualShotgunFirearmItem.h"

AManualShotgunFirearmItem::AManualShotgunFirearmItem()
{

}

void AManualShotgunFirearmItem::PlayFireEffects()
{
    for (uint8 i = 0; i < 10; ++i)
    {
        Super::PlayFireEffects();
    }
}