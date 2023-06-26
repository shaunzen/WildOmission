// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class SurfaceProperties : ModuleRules
{
    public SurfaceProperties(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Niagara" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject" });
    }
}