// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Vitals : ModuleRules
{
    public Vitals(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UMG" });
    }
}