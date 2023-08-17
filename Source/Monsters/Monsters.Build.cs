// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Monsters : ModuleRules
{
    public Monsters(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "GameplayTasks", "NavigationSystem", "Animation", "Despawner", "Inventory", "Vitals" });
    }
}