// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class EquipableItems : ModuleRules
{
    public EquipableItems(ReadOnlyTargetRules Target) : base(Target)
    {
        // TODO remove game settings here
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "GatherableResources", "Inventory" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Niagara", "GameSettings", "Durability", "SurfaceProperties", "Vitals" });
    }
}