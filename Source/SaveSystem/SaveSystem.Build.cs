// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class SaveSystem: ModuleRules
{
    public SaveSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Weather", "WorldGeneration" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "TimeOfDay" });
    }
}