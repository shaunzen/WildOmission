// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;

public class WildOmission : ModuleRules
{
	public WildOmission(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Niagara", "MetasoundEngine" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "OnlineSubsystem", "OnlineSubsystemSteam", "Vitals", "Inventory", "EquipableItems", "Interaction", "Despawner", "Helpers" });
	}
}
