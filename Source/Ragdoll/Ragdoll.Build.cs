// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Ragdoll : ModuleRules
{
    public Ragdoll(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "NavigationSystem",
            "Deployables",
            "Despawner",
            "Durability",
            "GatherableResources",
            "Interaction",
            "Inventory",
            "SavableObject",
            "WorldGeneration"
        });
    }
}