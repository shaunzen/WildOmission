// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Weapons : ModuleRules
{
    public Weapons(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Inventory", "Interaction" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Niagara", "GameSettings", "SurfaceProperties" });
    }
}