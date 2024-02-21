// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class WildOmissionEditorTarget : TargetRules
{
	public WildOmissionEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		bUseIris = true;

		ExtraModuleNames.AddRange( new string[] { "WildOmissionCore" } );
	}
}
