// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Ragdolls : ModuleRules
{
    public Ragdolls(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "NavigationSystem", "Despawner", "Durability", "GatherableResources", "Inventory", "SaveSystem" });
    }
}