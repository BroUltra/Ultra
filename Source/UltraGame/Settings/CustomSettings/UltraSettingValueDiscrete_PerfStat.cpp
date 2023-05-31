// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraSettingValueDiscrete_PerfStat.h"

#include "CommonUIVisibilitySubsystem.h"
#include "Performance/UltraPerformanceSettings.h"
#include "Performance/UltraPerformanceStatTypes.h"
#include "Settings/UltraSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraSettingValueDiscrete_PerfStat)

class ULocalPlayer;

#define LOCTEXT_NAMESPACE "UltraSettings"

//////////////////////////////////////////////////////////////////////

class FGameSettingEditCondition_PerfStatAllowed : public FGameSettingEditCondition
{
public:
	static TSharedRef<FGameSettingEditCondition_PerfStatAllowed> Get(EUltraDisplayablePerformanceStat Stat)
	{
		return MakeShared<FGameSettingEditCondition_PerfStatAllowed>(Stat);
	}

	FGameSettingEditCondition_PerfStatAllowed(EUltraDisplayablePerformanceStat Stat)
		: AssociatedStat(Stat)
	{
	}

	//~FGameSettingEditCondition interface
	virtual void GatherEditState(const ULocalPlayer* InLocalPlayer, FGameSettingEditableState& InOutEditState) const override
	{
		const FGameplayTagContainer& VisibilityTags = UCommonUIVisibilitySubsystem::GetChecked(InLocalPlayer)->GetVisibilityTags();

		bool bCanShowStat = false;
		for (const FUltraPerformanceStatGroup& Group : GetDefault<UUltraPerformanceSettings>()->UserFacingPerformanceStats) //@TODO: Move this stuff to per-platform instead of doing vis queries too?
		{
			if (Group.AllowedStats.Contains(AssociatedStat))
			{
				const bool bShowGroup = (Group.VisibilityQuery.IsEmpty() || Group.VisibilityQuery.Matches(VisibilityTags));
				if (bShowGroup)
				{
					bCanShowStat = true;
					break;
				}
			}
		}

		if (!bCanShowStat)
		{
			InOutEditState.Hide(TEXT("Stat is not listed in UUltraPerformanceSettings or is suppressed by current platform traits"));
		}
	}
	//~End of FGameSettingEditCondition interface

private:
	EUltraDisplayablePerformanceStat AssociatedStat;
};

//////////////////////////////////////////////////////////////////////

UUltraSettingValueDiscrete_PerfStat::UUltraSettingValueDiscrete_PerfStat()
{
}

void UUltraSettingValueDiscrete_PerfStat::SetStat(EUltraDisplayablePerformanceStat InStat)
{
	StatToDisplay = InStat;
	SetDevName(FName(*FString::Printf(TEXT("PerfStat_%d"), (int32)StatToDisplay)));
	AddEditCondition(FGameSettingEditCondition_PerfStatAllowed::Get(StatToDisplay));
}

void UUltraSettingValueDiscrete_PerfStat::AddMode(FText&& Label, EUltraStatDisplayMode Mode)
{
	Options.Emplace(MoveTemp(Label));
	DisplayModes.Add(Mode);
}

void UUltraSettingValueDiscrete_PerfStat::OnInitialized()
{
	Super::OnInitialized();

	AddMode(LOCTEXT("PerfStatDisplayMode_None", "None"), EUltraStatDisplayMode::Hidden);
	AddMode(LOCTEXT("PerfStatDisplayMode_TextOnly", "Text Only"), EUltraStatDisplayMode::TextOnly);
	AddMode(LOCTEXT("PerfStatDisplayMode_GraphOnly", "Graph Only"), EUltraStatDisplayMode::GraphOnly);
	AddMode(LOCTEXT("PerfStatDisplayMode_TextAndGraph", "Text and Graph"), EUltraStatDisplayMode::TextAndGraph);
}

void UUltraSettingValueDiscrete_PerfStat::StoreInitial()
{
	const UUltraSettingsLocal* Settings = UUltraSettingsLocal::Get();
	InitialMode = Settings->GetPerfStatDisplayState(StatToDisplay);
}

void UUltraSettingValueDiscrete_PerfStat::ResetToDefault()
{
	UUltraSettingsLocal* Settings = UUltraSettingsLocal::Get();
	Settings->SetPerfStatDisplayState(StatToDisplay, EUltraStatDisplayMode::Hidden);
	NotifySettingChanged(EGameSettingChangeReason::ResetToDefault);
}

void UUltraSettingValueDiscrete_PerfStat::RestoreToInitial()
{
	UUltraSettingsLocal* Settings = UUltraSettingsLocal::Get();
	Settings->SetPerfStatDisplayState(StatToDisplay, InitialMode);
	NotifySettingChanged(EGameSettingChangeReason::RestoreToInitial);
}

void UUltraSettingValueDiscrete_PerfStat::SetDiscreteOptionByIndex(int32 Index)
{
	if (DisplayModes.IsValidIndex(Index))
	{
		const EUltraStatDisplayMode DisplayMode = DisplayModes[Index];

		UUltraSettingsLocal* Settings = UUltraSettingsLocal::Get();
		Settings->SetPerfStatDisplayState(StatToDisplay, DisplayMode);
	}
	NotifySettingChanged(EGameSettingChangeReason::Change);
}

int32 UUltraSettingValueDiscrete_PerfStat::GetDiscreteOptionIndex() const
{
	const UUltraSettingsLocal* Settings = UUltraSettingsLocal::Get();
	return DisplayModes.Find(Settings->GetPerfStatDisplayState(StatToDisplay));
}

TArray<FText> UUltraSettingValueDiscrete_PerfStat::GetDiscreteOptions() const
{
	return Options;
}

#undef LOCTEXT_NAMESPACE
