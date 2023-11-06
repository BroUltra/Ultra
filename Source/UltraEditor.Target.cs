// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class UltraEditorTarget : TargetRules
{
	public UltraEditorTarget(TargetInfo Target) : base(Target)
	{
        Type = TargetType.Editor;

        ExtraModuleNames.AddRange(new string[] { "UltraGame", "UltraEditor" });

        if (!bBuildAllModules)
        {
            NativePointerMemberBehaviorOverride = PointerMemberBehavior.Disallow;
        }

        UltraGameTarget.ApplySharedUltraTargetSettings(this);

        // This is used for touch screen development along with the "Unreal Remote 2" app
        EnablePlugins.Add("RemoteSession");
    }
}