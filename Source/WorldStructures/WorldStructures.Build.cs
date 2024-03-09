// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class WorldStructures : ModuleRules
{
    public WorldStructures(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Deployables", "Inventory", "SavableObject", "WorldGeneration" });

        SetupIrisSupport(Target);
    }
}