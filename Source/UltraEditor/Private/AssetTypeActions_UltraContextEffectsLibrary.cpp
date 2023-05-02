// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetTypeActions_UltraContextEffectsLibrary.h"

#include "Feedback/ContextEffects/UltraContextEffectsLibrary.h"

class UClass;

#define LOCTEXT_NAMESPACE "AssetTypeActions"

UClass* FAssetTypeActions_UltraContextEffectsLibrary::GetSupportedClass() const
{
	return UUltraContextEffectsLibrary::StaticClass();
}

#undef LOCTEXT_NAMESPACE
