// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AudioMixerBlueprintLibrary.h"
#include "CommonLocalPlayer.h"
#include "Containers/UnrealString.h"
#include "GenericTeamAgentInterface.h"
#include "HAL/Platform.h"
#include "Teams/UltraTeamAgentInterface.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/WeakObjectPtrTemplates.h"

#include "UltraLocalPlayer.generated.h"

class APlayerController;
class UInputMappingContext;
class UUltraSettingsLocal;
class UUltraSettingsShared;
class UObject;
class UWorld;
struct FFrame;
struct FSwapAudioOutputResult;

/**
 * UUltraLocalPlayer
 */
UCLASS()
class ULTRAGAME_API UUltraLocalPlayer : public UCommonLocalPlayer, public IUltraTeamAgentInterface
{
	GENERATED_BODY()

public:

	UUltraLocalPlayer();

	//~UObject interface
	virtual void PostInitProperties() override;
	//~End of UObject interface

	//~UPlayer interface
	virtual void SwitchController(class APlayerController* PC) override;
	//~End of UPlayer interface

	//~ULocalPlayer interface
	virtual bool SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld) override;
	virtual void InitOnlineSession() override;
	//~End of ULocalPlayer interface

	//~IUltraTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnUltraTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of IUltraTeamAgentInterface interface

public:
	UFUNCTION()
	UUltraSettingsLocal* GetLocalSettings() const;

	UFUNCTION()
	UUltraSettingsShared* GetSharedSettings() const;

protected:
	void OnAudioOutputDeviceChanged(const FString& InAudioOutputDeviceId);
	
	UFUNCTION()
	void OnCompletedAudioDeviceSwap(const FSwapAudioOutputResult& SwapResult);

private:
	void OnPlayerControllerChanged(APlayerController* NewController);

	UFUNCTION()
	void OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

private:
	UPROPERTY(Transient)
	mutable TObjectPtr<UUltraSettingsShared> SharedSettings;

	UPROPERTY(Transient)
	mutable TObjectPtr<const UInputMappingContext> InputMappingContext;

	UPROPERTY()
	FOnUltraTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> LastBoundPC;
};
