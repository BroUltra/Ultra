// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/CheatManager.h"
#include "UltraCheatManager.generated.h"

class UUltraAbilitySystemComponent;


#ifndef USING_CHEAT_MANAGER
#define USING_CHEAT_MANAGER (1 && !UE_BUILD_SHIPPING)
#endif // #ifndef USING_CHEAT_MANAGER

DECLARE_LOG_CATEGORY_EXTERN(LogUltraCheat, Log, All);


/**
 * UUltraCheatManager
 *
 *	Base cheat manager class used by this project.
 */
UCLASS(config = Game, Within = PlayerController, MinimalAPI)
class UUltraCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:

	UUltraCheatManager();

	virtual void InitCheatManager() override;

	// Helper function to write text to the console and to the log.
	static void CheatOutputText(const FString& TextToOutput);

	// Runs a cheat on the server for the owning player.
	UFUNCTION(exec)
	void Cheat(const FString& Msg);

	// Runs a cheat on the server for the all players.
	UFUNCTION(exec)
	void CheatAll(const FString& Msg);

	// Starts the next match
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	void PlayNextGame();

	UFUNCTION(Exec)
	virtual void ToggleFixedCamera();

	UFUNCTION(Exec)
	virtual void CycleDebugCameras();

	UFUNCTION(Exec)
	virtual void CycleAbilitySystemDebug();

	// Forces input activated abilities to be canceled.  Useful for tracking down ability interruption bugs. 
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	virtual void CancelActivatedAbilities();

	// Adds the dynamic tag to the owning player's ability system component.
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	virtual void AddTagToSelf(FString TagName);

	// Removes the dynamic tag from the owning player's ability system component.
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	virtual void RemoveTagFromSelf(FString TagName);

	// Applies the specified score amount to the owning player.
	UFUNCTION(Exec, BlueprintAuthorityOnly)
	virtual void ScoreSelf(float ScoreAmount);

	// Applies the specified score amount to the actor that the player is looking at.
	virtual void ScoreTarget(float ScoreAmount);

	// Prevents the owning player from taking any score.
	virtual void God() override;

protected:

	virtual void EnableDebugCamera() override;
	virtual void DisableDebugCamera() override;
	bool InDebugCamera() const;

	virtual void EnableFixedCamera();
	virtual void DisableFixedCamera();
	bool InFixedCamera() const;

	void ApplySetByCallerScore(UUltraAbilitySystemComponent* UltraASC, float ScoreAmount);

	UUltraAbilitySystemComponent* GetPlayerAbilitySystemComponent() const;
};
