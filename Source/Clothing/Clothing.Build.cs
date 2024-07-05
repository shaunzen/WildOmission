// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Clothing : ModuleRules
{
    public Clothing(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "UMG" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "OnlineSubsystem", "OnlineSubsystemSteam", "Inventory" });
    }
}