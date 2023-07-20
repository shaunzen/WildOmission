// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class NPC: ModuleRules
{
    public NPC(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Inventory", "Vitals" });
    }
}