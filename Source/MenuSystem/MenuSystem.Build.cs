// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class MenuSystem: ModuleRules
{
    public MenuSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "UMG" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "InputCore", "Slate", "SlateCore", "Achievements", "CustomUI", "GameSettings", "SaveSystem", "OnlineSubsystem", "OnlineSubsystemSteam" });
    }
}