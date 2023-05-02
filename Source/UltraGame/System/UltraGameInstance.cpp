// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraGameInstance.h"

#include "CommonSessionSubsystem.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Engine/LocalPlayer.h"
#include "GameplayTagContainer.h"
#include "UltraGameplayTags.h"
#include "Misc/AssertionMacros.h"
#include "Player/UltraPlayerController.h"
#include "Templates/Casts.h"
#include "Engine/LocalPlayer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraGameInstance)

UUltraGameInstance::UUltraGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUltraGameInstance::Init()
{
	Super::Init();

	// Register our custom init states
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);

	if (ensure(ComponentManager))
	{
		const FUltraGameplayTags& GameplayTags = FUltraGameplayTags::Get();

		ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
		ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_DataInitialized);
	}
}

void UUltraGameInstance::Shutdown()
{
	Super::Shutdown();
}

AUltraPlayerController* UUltraGameInstance::GetPrimaryPlayerController() const
{
	return Cast<AUltraPlayerController>(Super::GetPrimaryPlayerController(false));
}

bool UUltraGameInstance::CanJoinRequestedSession() const
{
	// Temporary first pass:  Always return true
	// This will be fleshed out to check the player's state
	if (!Super::CanJoinRequestedSession())
	{
		return false;
	}
	return true;
}
