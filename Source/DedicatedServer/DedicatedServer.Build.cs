// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class DedicatedServer : ModuleRules
{
    public DedicatedServer(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject" });
    }
}