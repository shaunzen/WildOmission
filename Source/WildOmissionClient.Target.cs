// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class WildOmissionClientTarget : TargetRules
{
	public WildOmissionClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.AddRange( new string[] { "WildOmissionCore" } );
	}
}
