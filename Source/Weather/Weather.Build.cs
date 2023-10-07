// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Weather : ModuleRules
{
    public Weather(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "SaveSystem" }); // Needs Save system for the storms are "Savable Objects", however the save system also depends on this module
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Niagara" });
    }
}