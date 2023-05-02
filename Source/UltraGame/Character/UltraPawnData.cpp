// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraPawnData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraPawnData)

UUltraPawnData::UUltraPawnData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PawnClass = nullptr;
	InputConfig = nullptr;
	DefaultCameraMode = nullptr;
}

