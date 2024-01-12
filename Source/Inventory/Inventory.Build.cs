// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Inventory : ModuleRules
{
    public Inventory(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "UMG", "Despawner", "Interaction", "SavableObject" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "InputCore", "Slate", "SlateCore", "CustomUI", "WorldGeneration" });
    }
}