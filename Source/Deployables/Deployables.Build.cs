// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Deployables : ModuleRules
{
    public Deployables(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject" });
    }
}