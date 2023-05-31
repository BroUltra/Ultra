// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraPerformanceStatSubsystem.h"

#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/NetConnection.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "GameModes/UltraGameState.h"
#include "Performance/UltraPerformanceStatTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraPerformanceStatSubsystem)

class FSubsystemCollectionBase;

//////////////////////////////////////////////////////////////////////
// FUltraPerformanceStatCache

void FUltraPerformanceStatCache::StartCharting()
{
}

void FUltraPerformanceStatCache::ProcessFrame(const FFrameData& FrameData)
{
	CachedData = FrameData;
	CachedServerFPS = 0.0f;
	CachedPingMS = 0.0f;
	CachedPacketLossIncomingPercent = 0.0f;
	CachedPacketLossOutgoingPercent = 0.0f;
	CachedPacketRateIncoming = 0.0f;
	CachedPacketRateOutgoing = 0.0f;
	CachedPacketSizeIncoming = 0.0f;
	CachedPacketSizeOutgoing = 0.0f;

	if (UWorld* World = MySubsystem->GetGameInstance()->GetWorld())
	{
		if (const AUltraGameState* GameState = World->GetGameState<AUltraGameState>())
		{
			CachedServerFPS = GameState->GetServerFPS();
		}

		if (APlayerController* LocalPC = GEngine->GetFirstLocalPlayerController(World))
		{
			if (APlayerState* PS = LocalPC->GetPlayerState<APlayerState>())
			{
				CachedPingMS = PS->GetPingInMilliseconds();
			}

			if (UNetConnection* NetConnection = LocalPC->GetNetConnection())
			{
				const UNetConnection::FNetConnectionPacketLoss& InLoss = NetConnection->GetInLossPercentage();
				CachedPacketLossIncomingPercent = InLoss.GetAvgLossPercentage();
				const UNetConnection::FNetConnectionPacketLoss& OutLoss = NetConnection->GetOutLossPercentage();
				CachedPacketLossOutgoingPercent = OutLoss.GetAvgLossPercentage();

				CachedPacketRateIncoming = NetConnection->InPacketsPerSecond;
				CachedPacketRateOutgoing = NetConnection->OutPacketsPerSecond;

				CachedPacketSizeIncoming = (NetConnection->InPacketsPerSecond != 0) ? (NetConnection->InBytesPerSecond / (float)NetConnection->InPacketsPerSecond) : 0.0f;
				CachedPacketSizeOutgoing = (NetConnection->OutPacketsPerSecond != 0) ? (NetConnection->OutBytesPerSecond / (float)NetConnection->OutPacketsPerSecond) : 0.0f;
			}
		}
	}
}

void FUltraPerformanceStatCache::StopCharting()
{
}

double FUltraPerformanceStatCache::GetCachedStat(EUltraDisplayablePerformanceStat Stat) const
{
	static_assert((int32)EUltraDisplayablePerformanceStat::Count == 15, "Need to update this function to deal with new performance stats");
	switch (Stat)
	{
	case EUltraDisplayablePerformanceStat::ClientFPS:
		return (CachedData.TrueDeltaSeconds != 0.0) ? (1.0 / CachedData.TrueDeltaSeconds) : 0.0;
	case EUltraDisplayablePerformanceStat::ServerFPS:
		return CachedServerFPS;
	case EUltraDisplayablePerformanceStat::IdleTime:
		return CachedData.IdleSeconds;
	case EUltraDisplayablePerformanceStat::FrameTime:
		return CachedData.TrueDeltaSeconds;
	case EUltraDisplayablePerformanceStat::FrameTime_GameThread:
		return CachedData.GameThreadTimeSeconds;
	case EUltraDisplayablePerformanceStat::FrameTime_RenderThread:
		return CachedData.RenderThreadTimeSeconds;
	case EUltraDisplayablePerformanceStat::FrameTime_RHIThread:
		return CachedData.RHIThreadTimeSeconds;
	case EUltraDisplayablePerformanceStat::FrameTime_GPU:
		return CachedData.GPUTimeSeconds;
	case EUltraDisplayablePerformanceStat::Ping:
		return CachedPingMS;
	case EUltraDisplayablePerformanceStat::PacketLoss_Incoming:
		return CachedPacketLossIncomingPercent;
	case EUltraDisplayablePerformanceStat::PacketLoss_Outgoing:
		return CachedPacketLossOutgoingPercent;
	case EUltraDisplayablePerformanceStat::PacketRate_Incoming:
		return CachedPacketRateIncoming;
	case EUltraDisplayablePerformanceStat::PacketRate_Outgoing:
		return CachedPacketRateOutgoing;
	case EUltraDisplayablePerformanceStat::PacketSize_Incoming:
		return CachedPacketSizeIncoming;
	case EUltraDisplayablePerformanceStat::PacketSize_Outgoing:
		return CachedPacketSizeOutgoing;
	}

	return 0.0f;
}

//////////////////////////////////////////////////////////////////////
// UUltraPerformanceStatSubsystem

void UUltraPerformanceStatSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Tracker = MakeShared<FUltraPerformanceStatCache>(this);
	GEngine->AddPerformanceDataConsumer(Tracker);
}

void UUltraPerformanceStatSubsystem::Deinitialize()
{
	GEngine->RemovePerformanceDataConsumer(Tracker);
	Tracker.Reset();
}

double UUltraPerformanceStatSubsystem::GetCachedStat(EUltraDisplayablePerformanceStat Stat) const
{
	return Tracker->GetCachedStat(Stat);
}

