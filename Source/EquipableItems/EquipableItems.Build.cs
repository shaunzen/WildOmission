// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class EquipableItems : ModuleRules
{
    public EquipableItems(ReadOnlyTargetRules Target) : base(Target)
    {
        //PublicDependencyModuleNames.AddRange(new string[] {  });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Niagara", "Vitals", "Inventory", "Durability" });
    }
}