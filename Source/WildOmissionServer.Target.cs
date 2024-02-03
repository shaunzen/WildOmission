// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class WildOmissionServerTarget : TargetRules
{
	public WildOmissionServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		bUsesSteam = true;

		ExtraModuleNames.AddRange( new string[] { "WildOmissionCore" } );
	}
}
