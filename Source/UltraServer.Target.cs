// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class UltraServerTarget : TargetRules
{
	public UltraServerTarget(TargetInfo Target) : base(Target)
	{
        Type = TargetType.Server;

        ExtraModuleNames.AddRange(new string[] { "UltraGame" });

        UltraGameTarget.ApplySharedUltraTargetSettings(this);

        bUseChecksInShipping = true;
    }
}