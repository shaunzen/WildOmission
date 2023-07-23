// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Animals: ModuleRules
{
    public Animals(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "GameplayTasks", "NavigationSystem", "Inventory", "Vitals" });
    }
}