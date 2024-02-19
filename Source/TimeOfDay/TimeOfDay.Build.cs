// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class TimeOfDay: ModuleRules
{
    public TimeOfDay(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject" });

        SetupIrisSupport(Target);
    }
}