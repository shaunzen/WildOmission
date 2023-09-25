// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class GameMusic : ModuleRules
{
    public GameMusic(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "TimeOfDay" });
    }
}