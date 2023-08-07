// Copyright Telephone Studios. All Rights Reserved.


#include "Interfaces/SavableObject.h"

// Add default functionality here for any ISavableObject functions that are not pure virtual.

FName ISavableObject::GetIdentifier() const
{
    return NAME_None;
}
