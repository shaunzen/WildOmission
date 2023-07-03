// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class GatherableResources: ModuleRules
{
    public GatherableResources(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Interaction", "Inventory", "SaveSystem", "Weather" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject" });
    }
}