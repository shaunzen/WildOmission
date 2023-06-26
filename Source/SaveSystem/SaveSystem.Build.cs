// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class SaveSystem: ModuleRules
{
    public SaveSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
    }
}