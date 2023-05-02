// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraGameData.h"
#include "UltraAssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraGameData)

UUltraGameData::UUltraGameData()
{
}

const UUltraGameData& UUltraGameData::UUltraGameData::Get()
{
	return UUltraAssetManager::Get().GetGameData();
}
