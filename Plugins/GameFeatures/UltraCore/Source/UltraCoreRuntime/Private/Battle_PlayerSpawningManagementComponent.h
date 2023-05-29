// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/Array.h"
#include "Math/Rotator.h"
#include "Player/UltraPlayerSpawningManagerComponent.h"
#include "UObject/UObjectGlobals.h"

#include "Battle_PlayerSpawningManagementComponent.generated.h"

class AActor;
class AController;
class AUltraPlayerStart;
class UObject;

/**
 * 
 */
UCLASS()
class UBattle_PlayerSpawningManagementComponent : public UUltraPlayerSpawningManagerComponent
{
	GENERATED_BODY()

public:

	UBattle_PlayerSpawningManagementComponent(const FObjectInitializer& ObjectInitializer);

	virtual AActor* OnChoosePlayerStart(AController* Player, TArray<AUltraPlayerStart*>& PlayerStarts) override;
	virtual void OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation) override;

protected:

};
