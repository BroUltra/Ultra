// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/Array.h"
#include "GameSettingValueDiscrete.h"
#include "HAL/Platform.h"
#include "Internationalization/Text.h"
#include "Performance/UltraPerformanceStatTypes.h"
#include "UObject/UObjectGlobals.h"

#include "UltraSettingValueDiscrete_PerfStat.generated.h"

class UObject;

UCLASS()
class UUltraSettingValueDiscrete_PerfStat : public UGameSettingValueDiscrete
{
	GENERATED_BODY()
	
public:

	UUltraSettingValueDiscrete_PerfStat();

	void SetStat(EUltraDisplayablePerformanceStat InStat);

	/** UGameSettingValue */
	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;

	/** UGameSettingValueDiscrete */
	virtual void SetDiscreteOptionByIndex(int32 Index) override;
	virtual int32 GetDiscreteOptionIndex() const override;
	virtual TArray<FText> GetDiscreteOptions() const override;

protected:
	/** UGameSettingValue */
	virtual void OnInitialized() override;
	
	void AddMode(FText&& Label, EUltraStatDisplayMode Mode);
protected:
	TArray<FText> Options;
	TArray<EUltraStatDisplayMode> DisplayModes;

	EUltraDisplayablePerformanceStat StatToDisplay;
	EUltraStatDisplayMode InitialMode;
};