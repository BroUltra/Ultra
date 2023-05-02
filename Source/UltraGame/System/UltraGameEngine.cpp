// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraGameEngine.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraGameEngine)

class IEngineLoop;


UUltraGameEngine::UUltraGameEngine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUltraGameEngine::Init(IEngineLoop* InEngineLoop)
{
	Super::Init(InEngineLoop);
}

