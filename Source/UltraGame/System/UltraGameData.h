// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"

#include "UltraGameData.generated.h"

class UGameplayEffect;
class UObject;

/**
 * UUltraGameData
 *
 *	Non-mutable data asset that contains global game data.
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Ultra Game Data", ShortTooltip = "Data asset containing global game data."))
class UUltraGameData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UUltraGameData();

	// Returns the loaded game data.
	static const UUltraGameData& Get();

public:

	// Gameplay effect used to apply hit.  Uses SetByCaller for the score magnitude.
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects", meta = (DisplayName = "Score Gameplay Effect (SetByCaller)"))
	TSoftClassPtr<UGameplayEffect> ScoreGameplayEffect_SetByCaller;

	// Gameplay effect used to add and remove dynamic tags.
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects")
	TSoftClassPtr<UGameplayEffect> DynamicTagGameplayEffect;
};
