// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class WorldGeneration : ModuleRules
{
    public WorldGeneration(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "SaveSystem" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject" });
    }
}