// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/UnrealString.h"
#include "GameFramework/CheatManager.h"
#include "UObject/UObjectGlobals.h"

#include "UltraCosmeticCheats.generated.h"

class UUltraControllerComponent_CharacterParts;
class UObject;
struct FFrame;

/** Cheats related to bots */
UCLASS(NotBlueprintable)
class UUltraCosmeticCheats final : public UCheatManagerExtension
{
	GENERATED_BODY()

public:
	UUltraCosmeticCheats();

	// Adds a character part
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	void AddCharacterPart(const FString& AssetName, bool bSuppressNaturalParts = true);

	// Replaces previous cheat parts with a new one
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	void ReplaceCharacterPart(const FString& AssetName, bool bSuppressNaturalParts = true);

	// Clears any existing cheats
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	void ClearCharacterPartOverrides();

private:
	UUltraControllerComponent_CharacterParts* GetCosmeticComponent() const;
};
