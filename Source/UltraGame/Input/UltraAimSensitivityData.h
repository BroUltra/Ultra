// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/Map.h"
#include "Engine/DataAsset.h"
#include "Settings/UltraSettingsShared.h"
#include "UObject/UObjectGlobals.h"

#include "UltraAimSensitivityData.generated.h"

class UObject;

/** Defines a set of gamepad sensitivity to a float value. */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Ultra Aim Sensitivity Data", ShortTooltip = "Data asset used to define a map of Gamepad Sensitivty to a float value."))
class ULTRAGAME_API UUltraAimSensitivityData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UUltraAimSensitivityData(const FObjectInitializer& ObjectInitializer);
	
	const float SensitivtyEnumToFloat(const EUltraGamepadSensitivity InSensitivity) const;
	
protected:
	/** Map of SensitivityMap settings to their corresponding float */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EUltraGamepadSensitivity, float> SensitivityMap;
};