// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "GenericTeamAgentInterface.h"
#include "HAL/Platform.h"
#include "ModularAIController.h"
#include "Teams/UltraTeamAgentInterface.h"
#include "UObject/UObjectGlobals.h"

#include "UltraPlayerBotController.generated.h"

class APlayerState;
class UAIPerceptionComponent;
class UObject;
struct FFrame;

/**
 * AUltraPlayerBotController
 *
 *	The controller class used by player bots in this project.
 */
UCLASS(Blueprintable)
class AUltraPlayerBotController : public AModularAIController, public IUltraTeamAgentInterface
{
	GENERATED_BODY()

public:
	AUltraPlayerBotController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~IUltraTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnUltraTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~End of IUltraTeamAgentInterface interface

	// Attempts to restart this controller (e.g., to respawn it)
	void ServerRestartController();

	//Update Team Attitude for the AI
	UFUNCTION(BlueprintCallable, Category = "Ultra AI Player Controller")
	void UpdateTeamAttitude(UAIPerceptionComponent* AIPerception);

	virtual void OnUnPossess() override;


private:
	UFUNCTION()
	void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

protected:
	// Called when the player state is set or cleared
	virtual void OnPlayerStateChanged();

private:
	void BroadcastOnPlayerStateChanged();

protected:	
	//~AController interface
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	//~End of AController interface

private:
	UPROPERTY()
	FOnUltraTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;
};
