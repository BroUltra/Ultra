// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Camera/UltraCameraAssistInterface.h"
#include "CommonPlayerController.h"
#include "Teams/UltraTeamAgentInterface.h"

#include "UltraPlayerController.generated.h"

struct FGenericTeamId;

class AUltraHUD;
class AUltraPlayerState;
class APawn;
class APlayerState;
class FPrimitiveComponentId;
class IInputInterface;
class UUltraAbilitySystemComponent;
class UUltraSettingsShared;
class UObject;
class UPlayer;
struct FFrame;

/**
 * AUltraPlayerController
 *
 *	The base player controller class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base player controller class used by this project."))
class ULTRAGAME_API AUltraPlayerController : public ACommonPlayerController, public IUltraCameraAssistInterface, public IUltraTeamAgentInterface
{
	GENERATED_BODY()

public:

	AUltraPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Ultra|PlayerController")
	AUltraPlayerState* GetUltraPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "Ultra|PlayerController")
	UUltraAbilitySystemComponent* GetUltraAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Ultra|PlayerController")
	AUltraHUD* GetUltraHUD() const;

	// Run a cheat command on the server.
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCheat(const FString& Msg);

	// Run a cheat command on the server for all players.
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCheatAll(const FString& Msg);

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface

	//~AController interface
	virtual void OnUnPossess() override;
	//~End of AController interface

	//~APlayerController interface
	virtual void ReceivedPlayer() override;
	virtual void PlayerTick(float DeltaTime) override;
	//~End of APlayerController interface

	//~IUltraCameraAssistInterface interface
	virtual void OnCameraPenetratingTarget() override;
	//~End of IUltraCameraAssistInterface interface

	//~ACommonPlayerController interface
	virtual void OnPossess(APawn* InPawn) override;
	//~End of ACommonPlayerController interface
	
	//~IUltraTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnUltraTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of IUltraTeamAgentInterface interface

	UFUNCTION(BlueprintCallable, Category = "Ultra|Character")
	void SetIsAutoRunning(const bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Ultra|Character")
	bool GetIsAutoRunning() const;

private:
	UPROPERTY()
	FOnUltraTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;

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

	//~APlayerController interface
	virtual void SetPlayer(UPlayer* InPlayer) override;
	virtual void AddCheats(bool bForce) override;

	virtual void UpdateForceFeedback(IInputInterface* InputInterface, const int32 ControllerId) override;
	virtual void UpdateHiddenComponents(const FVector& ViewLocation, TSet<FPrimitiveComponentId>& OutHiddenComponents) override;

	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~End of APlayerController interface

	void OnSettingsChanged(UUltraSettingsShared* Settings);
	
	void OnStartAutoRun();
	void OnEndAutoRun();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnStartAutoRun"))
	void K2_OnStartAutoRun();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnEndAutoRun"))
	void K2_OnEndAutoRun();

	bool bHideViewTargetPawnNextFrame = false;
};


// A player controller used for replay capture and playback
UCLASS()
class AUltraReplayPlayerController : public AUltraPlayerController
{
	GENERATED_BODY()

	virtual void SetPlayer(UPlayer* InPlayer) override;
};
