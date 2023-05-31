// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DataSource/GameSettingDataSourceDynamic.h" // IWYU pragma: keep
#include "GameSettingRegistry.h"
#include "Settings/UltraSettingsLocal.h" // IWYU pragma: keep

#include "UltraGameSettingRegistry.generated.h"

class ULocalPlayer;
class UObject;

//--------------------------------------
// UUltraGameSettingRegistry
//--------------------------------------

class UGameSettingCollection;
class UUltraLocalPlayer;

DECLARE_LOG_CATEGORY_EXTERN(LogUltraGameSettingRegistry, Log, Log);

#define GET_SHARED_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)							\
	MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({								\
		GET_FUNCTION_NAME_STRING_CHECKED(UUltraLocalPlayer, GetSharedSettings),				\
		GET_FUNCTION_NAME_STRING_CHECKED(UUltraSettingsShared, FunctionOrPropertyName)		\
	}))

#define GET_LOCAL_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)							\
	MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({								\
		GET_FUNCTION_NAME_STRING_CHECKED(UUltraLocalPlayer, GetLocalSettings),				\
		GET_FUNCTION_NAME_STRING_CHECKED(UUltraSettingsLocal, FunctionOrPropertyName)		\
	}))

/**
 * 
 */
UCLASS()
class UUltraGameSettingRegistry : public UGameSettingRegistry
{
	GENERATED_BODY()

public:
	UUltraGameSettingRegistry();

	static UUltraGameSettingRegistry* Get(UUltraLocalPlayer* InLocalPlayer);
	
	virtual void SaveChanges() override;

protected:
	virtual void OnInitialize(ULocalPlayer* InLocalPlayer) override;
	virtual bool IsFinishedInitializing() const override;

	UGameSettingCollection* InitializeVideoSettings(UUltraLocalPlayer* InLocalPlayer);
	void InitializeVideoSettings_FrameRates(UGameSettingCollection* Screen, UUltraLocalPlayer* InLocalPlayer);
	void AddPerformanceStatPage(UGameSettingCollection* Screen, UUltraLocalPlayer* InLocalPlayer);

	UGameSettingCollection* InitializeAudioSettings(UUltraLocalPlayer* InLocalPlayer);
	UGameSettingCollection* InitializeGameplaySettings(UUltraLocalPlayer* InLocalPlayer);

	UGameSettingCollection* InitializeMouseAndKeyboardSettings(UUltraLocalPlayer* InLocalPlayer);
	UGameSettingCollection* InitializeGamepadSettings(UUltraLocalPlayer* InLocalPlayer);

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> VideoSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> AudioSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> GameplaySettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> MouseAndKeyboardSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> GamepadSettings;
};
