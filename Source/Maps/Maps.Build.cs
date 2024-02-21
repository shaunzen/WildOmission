// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Maps : ModuleRules
{
    public Maps(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "UMG", "SavableObject" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "InputCore", "Slate", "SlateCore", "EquipableItems", "WorldGeneration" });

        SetupIrisSupport(Target);
    }
}