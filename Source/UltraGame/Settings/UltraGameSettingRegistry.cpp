// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraGameSettingRegistry.h"

#include "GameSettingCollection.h"
#include "UltraSettingsLocal.h"
#include "UltraSettingsShared.h"
#include "Player/UltraLocalPlayer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraGameSettingRegistry)

DEFINE_LOG_CATEGORY(LogUltraGameSettingRegistry);

#define LOCTEXT_NAMESPACE "Ultra"

//--------------------------------------
// UUltraGameSettingRegistry
//--------------------------------------

UUltraGameSettingRegistry::UUltraGameSettingRegistry()
{
}

UUltraGameSettingRegistry* UUltraGameSettingRegistry::Get(UUltraLocalPlayer* InLocalPlayer)
{
	UUltraGameSettingRegistry* Registry = FindObject<UUltraGameSettingRegistry>(InLocalPlayer, TEXT("UltraGameSettingRegistry"), true);
	if (Registry == nullptr)
	{
		Registry = NewObject<UUltraGameSettingRegistry>(InLocalPlayer, TEXT("UltraGameSettingRegistry"));
		Registry->Initialize(InLocalPlayer);
	}

	return Registry;
}

bool UUltraGameSettingRegistry::IsFinishedInitializing() const
{
	if (Super::IsFinishedInitializing())
	{
		if (UUltraLocalPlayer* LocalPlayer = Cast<UUltraLocalPlayer>(OwningLocalPlayer))
		{
			if (LocalPlayer->GetSharedSettings() == nullptr)
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

void UUltraGameSettingRegistry::OnInitialize(ULocalPlayer* InLocalPlayer)
{
	UUltraLocalPlayer* UltraLocalPlayer = Cast<UUltraLocalPlayer>(InLocalPlayer);

	VideoSettings = InitializeVideoSettings(UltraLocalPlayer);
	InitializeVideoSettings_FrameRates(VideoSettings, UltraLocalPlayer);
	RegisterSetting(VideoSettings);

	AudioSettings = InitializeAudioSettings(UltraLocalPlayer);
	RegisterSetting(AudioSettings);

	GameplaySettings = InitializeGameplaySettings(UltraLocalPlayer);
	RegisterSetting(GameplaySettings);

	MouseAndKeyboardSettings = InitializeMouseAndKeyboardSettings(UltraLocalPlayer);
	RegisterSetting(MouseAndKeyboardSettings);

	GamepadSettings = InitializeGamepadSettings(UltraLocalPlayer);
	RegisterSetting(GamepadSettings);
}

void UUltraGameSettingRegistry::SaveChanges()
{
	Super::SaveChanges();
	
	if (UUltraLocalPlayer* LocalPlayer = Cast<UUltraLocalPlayer>(OwningLocalPlayer))
	{
		// Game user settings need to be applied to handle things like resolution, this saves indirectly
		LocalPlayer->GetLocalSettings()->ApplySettings(false);
		
		LocalPlayer->GetSharedSettings()->ApplySettings();
		LocalPlayer->GetSharedSettings()->SaveSettings();
	}
}

#undef LOCTEXT_NAMESPACE

