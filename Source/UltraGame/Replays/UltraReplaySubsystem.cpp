// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraReplaySubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Engine/DemoNetDriver.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraReplaySubsystem)

UUltraReplaySubsystem::UUltraReplaySubsystem()
{
}

void UUltraReplaySubsystem::PlayReplay(UUltraReplayListEntry* Replay)
{
	if (Replay != nullptr)
	{
		FString DemoName = Replay->StreamInfo.Name;
		GetGameInstance()->PlayReplay(DemoName);
	}
}

// void UUltraReplaySubsystem::DeleteReplay()
// {
//	ReplayStreamer->DeleteFinishedStream(SelectedItem->StreamInfo.Name, FDeleteFinishedStreamCallback::CreateSP(this, &SUltraDemoList::OnDeleteFinishedStreamComplete));
// }

void UUltraReplaySubsystem::SeekInActiveReplay(float TimeInSeconds)
{
	if (UDemoNetDriver* DemoDriver = GetDemoDriver())
	{
		DemoDriver->GotoTimeInSeconds(TimeInSeconds);
	}
}

float UUltraReplaySubsystem::GetReplayLengthInSeconds() const
{
	if (UDemoNetDriver* DemoDriver = GetDemoDriver())
	{
		return DemoDriver->GetDemoTotalTime();
	}
	return 0.0f;
}

float UUltraReplaySubsystem::GetReplayCurrentTime() const
{
	if (UDemoNetDriver* DemoDriver = GetDemoDriver())
	{
		return DemoDriver->GetDemoCurrentTime();
	}
	return 0.0f;
}

UDemoNetDriver* UUltraReplaySubsystem::GetDemoDriver() const
{
	if (UWorld* World = GetGameInstance()->GetWorld())
	{
		return World->GetDemoNetDriver();
	}
	return nullptr;
}

