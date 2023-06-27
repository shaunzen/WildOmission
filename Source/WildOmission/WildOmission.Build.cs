// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class WildOmission : ModuleRules
{
	public WildOmission(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Niagara", "MetasoundEngine", "SaveSystem", "SurfaceProperties", "Vitals", "Inventory", "GatherableResources", "EquipableItems", "Interaction", "Despawner", "Durability", "Helpers" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "OnlineSubsystem", "OnlineSubsystemSteam" });
	}
}
