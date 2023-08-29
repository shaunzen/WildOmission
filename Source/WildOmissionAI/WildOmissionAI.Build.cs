// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class WildOmissionAI : ModuleRules
{
    public WildOmissionAI(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "GameplayTasks", "Vitals" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "NavigationSystem", "Despawner", "Inventory" });
    }
}