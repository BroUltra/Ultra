// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraPerfStatWidgetBase.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Performance/UltraPerformanceStatSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraPerfStatWidgetBase)

//////////////////////////////////////////////////////////////////////
// UUltraPerfStatWidgetBase

UUltraPerfStatWidgetBase::UUltraPerfStatWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

double UUltraPerfStatWidgetBase::FetchStatValue()
{
	if (CachedStatSubsystem == nullptr)
	{
		if (UWorld* World = GetWorld())
		{
			if (UGameInstance* GameInstance = World->GetGameInstance())
			{
				CachedStatSubsystem = GameInstance->GetSubsystem<UUltraPerformanceStatSubsystem>();
			}
		}
	}

	if (CachedStatSubsystem)
	{
		return CachedStatSubsystem->GetCachedStat(StatToDisplay);
	}
	else
	{
		return 0.0;
	}
}

