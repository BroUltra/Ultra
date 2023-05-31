// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraRuntimeOptions.h"

#include "UObject/Class.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraRuntimeOptions)

UUltraRuntimeOptions::UUltraRuntimeOptions()
{
	OptionCommandPrefix = TEXT("ro");
}

UUltraRuntimeOptions* UUltraRuntimeOptions::GetRuntimeOptions()
{
	return GetMutableDefault<UUltraRuntimeOptions>();
}

const UUltraRuntimeOptions& UUltraRuntimeOptions::Get()
{
	const UUltraRuntimeOptions& RuntimeOptions = *GetDefault<UUltraRuntimeOptions>();
	return RuntimeOptions;
}