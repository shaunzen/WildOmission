// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class WildOmission : ModuleRules
{
	public WildOmission(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", 
			"InputCore", "EnhancedInput", "UMG", 
			"Niagara", "MetasoundEngine", "Crafting",
			"Deployables", "Despawner", "Durability",
			"EquipableItems", "GameChat", "GatherableResources",
			"Helpers", "Interaction", "Inventory", "SaveSystem",
			"SurfaceProperties", "Vitals", "Weather",
			"WorldGeneration"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "OnlineSubsystem", "OnlineSubsystemSteam" });
	}
}
