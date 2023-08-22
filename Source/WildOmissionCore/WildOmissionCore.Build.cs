// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class WildOmissionCore : ModuleRules
{
	public WildOmissionCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine",
			"InputCore", "EnhancedInput", "UMG", 
			"Niagara", "NavigationSystem", "MetasoundEngine", "Animals", "Animation", "Crafting",
			"CustomUI", "Deployables", "Despawner", "Durability",
			"EquipableItems", "GameChat", "GameSettings", "GatherableResources",
			"Interaction", "Inventory", "MenuSystem", "Monsters", "Notifications", "SaveSystem",
			"SurfaceProperties", "TimeOfDay", "Vitals", "Weapons", "Weather",
			"WorldGeneration"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "OnlineSubsystem", "OnlineSubsystemSteam" });
	}
}
