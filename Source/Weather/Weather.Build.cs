// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Weather: ModuleRules
{
    public Weather(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "SaveSystem", "WorldGeneration" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Niagara" });
    }
}