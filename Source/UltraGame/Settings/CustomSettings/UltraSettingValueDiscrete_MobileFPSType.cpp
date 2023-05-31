// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraSettingValueDiscrete_MobileFPSType.h"

#include "Performance/UltraPerformanceSettings.h"
#include "Settings/UltraSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraSettingValueDiscrete_MobileFPSType)

#define LOCTEXT_NAMESPACE "UltraSettings"

UUltraSettingValueDiscrete_MobileFPSType::UUltraSettingValueDiscrete_MobileFPSType()
{
}

void UUltraSettingValueDiscrete_MobileFPSType::OnInitialized()
{
	Super::OnInitialized();

	const UUltraPlatformSpecificRenderingSettings* PlatformSettings = UUltraPlatformSpecificRenderingSettings::Get();
	const UUltraSettingsLocal* UserSettings = UUltraSettingsLocal::Get();

	for (int32 TestLimit : PlatformSettings->MobileFrameRateLimits)
	{
		if (UUltraSettingsLocal::IsSupportedMobileFramePace(TestLimit))
		{
			FPSOptions.Add(TestLimit, MakeLimitString(TestLimit));
		}
	}

	const int32 FirstFrameRateWithQualityLimit = UserSettings->GetFirstFrameRateWithQualityLimit();
	if (FirstFrameRateWithQualityLimit > 0)
	{
		SetWarningRichText(FText::Format(LOCTEXT("MobileFPSType_Note", "<strong>Note: Changing the framerate setting to {0} or higher might lower your Quality Presets.</>"), MakeLimitString(FirstFrameRateWithQualityLimit)));
	}
}

int32 UUltraSettingValueDiscrete_MobileFPSType::GetDefaultFPS() const
{
	return UUltraSettingsLocal::GetDefaultMobileFrameRate();
}

FText UUltraSettingValueDiscrete_MobileFPSType::MakeLimitString(int32 Number)
{
	return FText::Format(LOCTEXT("MobileFrameRateOption", "{0} FPS"), FText::AsNumber(Number));
}

void UUltraSettingValueDiscrete_MobileFPSType::StoreInitial()
{
	InitialValue = GetValue();
}

void UUltraSettingValueDiscrete_MobileFPSType::ResetToDefault()
{
	SetValue(GetDefaultFPS(), EGameSettingChangeReason::ResetToDefault);
}

void UUltraSettingValueDiscrete_MobileFPSType::RestoreToInitial()
{
	SetValue(InitialValue, EGameSettingChangeReason::RestoreToInitial);
}

void UUltraSettingValueDiscrete_MobileFPSType::SetDiscreteOptionByIndex(int32 Index)
{
	TArray<int32> FPSOptionsModes;
	FPSOptions.GenerateKeyArray(FPSOptionsModes);

	int32 NewMode = FPSOptionsModes.IsValidIndex(Index) ? FPSOptionsModes[Index] : GetDefaultFPS();

	SetValue(NewMode, EGameSettingChangeReason::Change);
}

int32 UUltraSettingValueDiscrete_MobileFPSType::GetDiscreteOptionIndex() const
{
	TArray<int32> FPSOptionsModes;
	FPSOptions.GenerateKeyArray(FPSOptionsModes);
	return FPSOptionsModes.IndexOfByKey(GetValue());
}

TArray<FText> UUltraSettingValueDiscrete_MobileFPSType::GetDiscreteOptions() const
{
	TArray<FText> Options;
	FPSOptions.GenerateValueArray(Options);

	return Options;
}

int32 UUltraSettingValueDiscrete_MobileFPSType::GetValue() const
{
	return UUltraSettingsLocal::Get()->GetDesiredMobileFrameRateLimit();
}

void UUltraSettingValueDiscrete_MobileFPSType::SetValue(int32 NewLimitFPS, EGameSettingChangeReason InReason)
{
	UUltraSettingsLocal::Get()->SetDesiredMobileFrameRateLimit(NewLimitFPS);

	NotifySettingChanged(InReason);
}

#undef LOCTEXT_NAMESPACE

