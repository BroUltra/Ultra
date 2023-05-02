// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraInputComponent.h"

#include "Containers/Map.h"
#include "EnhancedInputSubsystems.h"
#include "Input/UltraMappableConfigPair.h"
#include "InputCoreTypes.h"
#include "Player/UltraLocalPlayer.h"
#include "Settings/UltraSettingsLocal.h"
#include "UObject/NameTypes.h"
#include "UObject/UnrealNames.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraInputComponent)

class UUltraInputConfig;

UUltraInputComponent::UUltraInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UUltraInputComponent::AddInputMappings(const UUltraInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	UUltraLocalPlayer* LocalPlayer = InputSubsystem->GetLocalPlayer<UUltraLocalPlayer>();
	check(LocalPlayer);

	// Add any registered input mappings from the settings!
	if (UUltraSettingsLocal* LocalSettings = UUltraSettingsLocal::Get())
	{	
		// Tell enhanced input about any custom keymappings that the player may have customized
		for (const TPair<FName, FKey>& Pair : LocalSettings->GetCustomPlayerInputConfig())
		{
			if (Pair.Key != NAME_None && Pair.Value.IsValid())
			{
				InputSubsystem->AddPlayerMappedKey(Pair.Key, Pair.Value);
			}
		}
	}
}

void UUltraInputComponent::RemoveInputMappings(const UUltraInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	UUltraLocalPlayer* LocalPlayer = InputSubsystem->GetLocalPlayer<UUltraLocalPlayer>();
	check(LocalPlayer);
	
	if (UUltraSettingsLocal* LocalSettings = UUltraSettingsLocal::Get())
	{
		// Remove any registered input contexts
		const TArray<FLoadedMappableConfigPair>& Configs = LocalSettings->GetAllRegisteredInputConfigs();
		for (const FLoadedMappableConfigPair& Pair : Configs)
		{
			InputSubsystem->RemovePlayerMappableConfig(Pair.Config);
		}
		
		// Clear any player mapped keys from enhanced input
		for (const TPair<FName, FKey>& Pair : LocalSettings->GetCustomPlayerInputConfig())
		{
			InputSubsystem->RemovePlayerMappedKey(Pair.Key);
		}
	}
}

void UUltraInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
