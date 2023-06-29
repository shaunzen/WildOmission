// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Helpers: ModuleRules
{
    public Helpers(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject" });
    }
}