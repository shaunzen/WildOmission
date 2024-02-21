// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Monsters : ModuleRules
{
    public Monsters(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "GameplayTasks", "Niagara",
            "EquipableItems", "Inventory", "SaveSystem", "TimeOfDay", "Vitals", "Weapons", "WildOmissionAI" });

        SetupIrisSupport(Target);
    }
}