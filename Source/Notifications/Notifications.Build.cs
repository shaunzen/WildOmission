// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class Notifications: ModuleRules
{
    public Notifications(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "UMG" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "CustomUI", "Deployables", "Inventory", "Vitals" });
    }
}