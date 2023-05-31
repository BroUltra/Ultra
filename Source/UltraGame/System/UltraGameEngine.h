// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/GameEngine.h"

#include "UltraGameEngine.generated.h"

class IEngineLoop;
class UObject;


UCLASS()
class UUltraGameEngine : public UGameEngine
{
	GENERATED_BODY()

public:

	UUltraGameEngine(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void Init(IEngineLoop* InEngineLoop) override;
};
