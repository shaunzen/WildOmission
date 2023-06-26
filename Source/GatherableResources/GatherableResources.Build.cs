// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class GatherableResources: ModuleRules
{
    public GatherableResources(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Interaction", "Inventory" });
    }
}