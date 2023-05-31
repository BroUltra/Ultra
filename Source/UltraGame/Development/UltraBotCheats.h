// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/CheatManager.h"

#include "UltraBotCheats.generated.h"

class UUltraBotCreationComponent;
class UObject;
struct FFrame;

/** Cheats related to bots */
UCLASS(NotBlueprintable)
class UUltraBotCheats final : public UCheatManagerExtension
{
	GENERATED_BODY()

public:
	UUltraBotCheats();

	// Adds a bot player
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	void AddPlayerBot();

	// Removes a random bot player
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	void RemovePlayerBot();

private:
	UUltraBotCreationComponent* GetBotComponent() const;
};
