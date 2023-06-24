// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Inventory : ModuleRules

{

    public Inventory(ReadOnlyTargetRules Target) : base(Target)

    {

        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "Engine", "UMG", "Interactable", "Helpers" });

    }

}