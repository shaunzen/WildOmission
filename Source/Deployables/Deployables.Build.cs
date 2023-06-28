// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Deployables : ModuleRules
{
    public Deployables(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "UMG", "Inventory", "EquipableItems" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "GatherableResources", "Durability", "Despawner", "Interaction", "SaveSystem" });
    }
}