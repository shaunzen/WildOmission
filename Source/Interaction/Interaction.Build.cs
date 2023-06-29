// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Interaction : ModuleRules
{ 
    public Interaction(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject" });
    }
}