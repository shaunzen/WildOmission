// Copyright Telephone Studios. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class WildOmissionTarget : TargetRules
{
	public WildOmissionTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		bUsesSteam = true;
		bUseIris = true;

		ExtraModuleNames.AddRange( new string[] { "WildOmissionCore" } );

        GlobalDefinitions.Add("UE_PROJECT_STEAMSHIPPINGID=2348700");
        GlobalDefinitions.Add("UE_PROJECT_STEAMPRODUCTNAME=\"Wild Omission\"");
        GlobalDefinitions.Add("UE_PROJECT_STEAMGAMEDIR=\"Wild Omission\"");
        GlobalDefinitions.Add("UE_PROJECT_STEAMGAMEDESC=\"Wild Omission\"");
    }
}
