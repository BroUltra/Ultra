// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ChartCreation.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "UltraPerformanceStatSubsystem.generated.h"

enum class EUltraDisplayablePerformanceStat : uint8;

class FSubsystemCollectionBase;
class UUltraPerformanceStatSubsystem;
class UObject;
struct FFrame;

//////////////////////////////////////////////////////////////////////

// Observer which caches the stats for the previous frame
struct FUltraPerformanceStatCache : public IPerformanceDataConsumer
{
public:
	FUltraPerformanceStatCache(UUltraPerformanceStatSubsystem* InSubsystem)
		: MySubsystem(InSubsystem)
	{
	}

	//~IPerformanceDataConsumer interface
	virtual void StartCharting() override;
	virtual void ProcessFrame(const FFrameData& FrameData) override;
	virtual void StopCharting() override;
	//~End of IPerformanceDataConsumer interface

	double GetCachedStat(EUltraDisplayablePerformanceStat Stat) const;

protected:
	IPerformanceDataConsumer::FFrameData CachedData;
	UUltraPerformanceStatSubsystem* MySubsystem;

	float CachedServerFPS = 0.0f;
	float CachedPingMS = 0.0f;
	float CachedPacketLossIncomingPercent = 0.0f;
	float CachedPacketLossOutgoingPercent = 0.0f;
	float CachedPacketRateIncoming = 0.0f;
	float CachedPacketRateOutgoing = 0.0f;
	float CachedPacketSizeIncoming = 0.0f;
	float CachedPacketSizeOutgoing = 0.0f;
};

//////////////////////////////////////////////////////////////////////

// Subsystem to allow access to performance stats for display purposes
UCLASS(BlueprintType)
class UUltraPerformanceStatSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	double GetCachedStat(EUltraDisplayablePerformanceStat Stat) const;

	//~USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//~End of USubsystem interface

protected:
	TSharedPtr<FUltraPerformanceStatCache> Tracker;
};
