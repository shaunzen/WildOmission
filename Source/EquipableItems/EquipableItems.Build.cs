// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class EquipableItems : ModuleRules
{
    public EquipableItems(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Inventory" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Niagara", "Durability", "GatherableResources", "SurfaceProperties", "Vitals" });
    }
}