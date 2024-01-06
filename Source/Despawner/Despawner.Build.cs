// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Despawner : ModuleRules
{
    public Despawner(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "SavableObject" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "WorldGeneration" });
    }
}