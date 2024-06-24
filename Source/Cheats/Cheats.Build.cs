// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Cheats : ModuleRules
{
    public Cheats(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "UMG" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject" });
    }
}