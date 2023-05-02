// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/GameSession.h"
#include "UObject/UObjectGlobals.h"

#include "UltraGameSession.generated.h"

class UObject;


UCLASS(Config = Game)
class AUltraGameSession : public AGameSession
{
	GENERATED_BODY()

public:

	AUltraGameSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
};
