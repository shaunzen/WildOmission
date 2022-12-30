// (c) 2023 Telephone Studios, all rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class WildOmissionTarget : TargetRules
{
	public WildOmissionTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.AddRange( new string[] { "WildOmission" } );
	}
}
