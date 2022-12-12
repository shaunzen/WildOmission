// (c) 2023 Telephone Studios, all rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class WildOmissionEditorTarget : TargetRules
{
	public WildOmissionEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "WildOmission" } );
	}
}
