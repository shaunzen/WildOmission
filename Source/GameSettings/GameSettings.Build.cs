// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class GameSettings : ModuleRules
{
    public GameSettings(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "UMG" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "InputCore", "EnhancedInput", "CustomUI", "Slate", "SlateCore" });
    }
}