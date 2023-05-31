// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Logging/LogMacros.h"

class UObject;

ULTRAGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogUltra, Log, All);
ULTRAGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogUltraExperience, Log, All);
ULTRAGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogUltraAbilitySystem, Log, All);
ULTRAGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogUltraTeams, Log, All);

ULTRAGAME_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
