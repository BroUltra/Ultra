// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/GameSession.h"

#include "UltraGameSession.generated.h"

class UObject;


UCLASS(Config = Game)
class AUltraGameSession : public AGameSession
{
	GENERATED_BODY()

public:

	AUltraGameSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	/** Override to disable the default behavior */
	virtual bool ProcessAutoLogin() override;
	
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
};
