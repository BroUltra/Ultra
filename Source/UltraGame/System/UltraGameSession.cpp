// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraGameSession.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraGameSession)


AUltraGameSession::AUltraGameSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool AUltraGameSession::ProcessAutoLogin()
{
	// This is actually handled in UltraGameMode::TryDedicatedServerLogin
	return true;
}

void AUltraGameSession::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void AUltraGameSession::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}

