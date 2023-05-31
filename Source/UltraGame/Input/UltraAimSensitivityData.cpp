// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraAimSensitivityData.h"

#include "Settings/UltraSettingsShared.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraAimSensitivityData)

UUltraAimSensitivityData::UUltraAimSensitivityData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SensitivityMap =
	{
		{ EUltraGamepadSensitivity::Slow,			0.5f },
		{ EUltraGamepadSensitivity::SlowPlus,		0.75f },
		{ EUltraGamepadSensitivity::SlowPlusPlus,	0.9f },
		{ EUltraGamepadSensitivity::Normal,		1.0f },
		{ EUltraGamepadSensitivity::NormalPlus,	1.1f },
		{ EUltraGamepadSensitivity::NormalPlusPlus,1.25f },
		{ EUltraGamepadSensitivity::Fast,			1.5f },
		{ EUltraGamepadSensitivity::FastPlus,		1.75f },
		{ EUltraGamepadSensitivity::FastPlusPlus,	2.0f },
		{ EUltraGamepadSensitivity::Insane,		2.5f },
	};
}

const float UUltraAimSensitivityData::SensitivtyEnumToFloat(const EUltraGamepadSensitivity InSensitivity) const
{
	if (const float* Sens = SensitivityMap.Find(InSensitivity))
	{
		return *Sens;
	}

	return 1.0f;
}

