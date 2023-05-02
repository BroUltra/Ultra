// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraPerformanceSettings.h"

#include "Engine/PlatformSettings.h"
#include "Engine/PlatformSettingsManager.h"
#include "Misc/AssertionMacros.h"
#include "Misc/EnumRange.h"
#include "UObject/NameTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraPerformanceSettings)

//////////////////////////////////////////////////////////////////////

UUltraPlatformSpecificRenderingSettings::UUltraPlatformSpecificRenderingSettings()
{
	MobileFrameRateLimits.Append({ 20, 30, 45, 60, 90, 120 });
}

const UUltraPlatformSpecificRenderingSettings* UUltraPlatformSpecificRenderingSettings::Get()
{
	UUltraPlatformSpecificRenderingSettings* Result = UPlatformSettingsManager::Get().GetSettingsForPlatform<ThisClass>();
	check(Result);
	return Result;
}

//////////////////////////////////////////////////////////////////////

UUltraPerformanceSettings::UUltraPerformanceSettings()
{
	PerPlatformSettings.Initialize(UUltraPlatformSpecificRenderingSettings::StaticClass());

	CategoryName = TEXT("Game");

	DesktopFrameRateLimits.Append({ 30, 60, 120, 144, 160, 165, 180, 200, 240, 360 });

	// Default to all stats are allowed
	FUltraPerformanceStatGroup& StatGroup = UserFacingPerformanceStats.AddDefaulted_GetRef();
	for (EUltraDisplayablePerformanceStat PerfStat : TEnumRange<EUltraDisplayablePerformanceStat>())
	{
		StatGroup.AllowedStats.Add(PerfStat);
	}
}

