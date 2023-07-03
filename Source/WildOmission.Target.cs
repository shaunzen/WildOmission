// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class WildOmissionTarget : TargetRules
{
	public WildOmissionTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		bUsesSteam = true;

		ExtraModuleNames.AddRange( new string[] { "WildOmissionCore" } );
	}
}
