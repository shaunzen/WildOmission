// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Durability : ModuleRules
{
    public Durability(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
    }
}