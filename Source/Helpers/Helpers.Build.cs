// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Helpers: ModuleRules
{
    public Helpers(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
    }
}