// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class UltraClientTarget : TargetRules
{
	public UltraClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;

		ExtraModuleNames.AddRange(new string[] { "UltraGame" });

		UltraGameTarget.ApplySharedUltraTargetSettings(this);
	}
}