// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraDebugCameraController.h"
#include "UltraCheatManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraDebugCameraController)


AUltraDebugCameraController::AUltraDebugCameraController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Use the same cheat class as UltraPlayerController to allow toggling the debug camera through cheats.
	CheatClass = UUltraCheatManager::StaticClass();
}

void AUltraDebugCameraController::AddCheats(bool bForce)
{
	// Mirrors UltraPlayerController's AddCheats() to avoid the player becoming stuck in the debug camera.
#if USING_CHEAT_MANAGER
	Super::AddCheats(true);
#else //#if USING_CHEAT_MANAGER
	Super::AddCheats(bForce);
#endif // #else //#if USING_CHEAT_MANAGER
}

