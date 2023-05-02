// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraEditorEngine.h"

#include "Development/UltraDeveloperSettings.h"
#include "Development/UltraPlatformEmulationSettings.h"
#include "Engine/World.h"
#include "Framework/Commands/InputBindingManager.h"
#include "Framework/Commands/InputChord.h"
#include "Framework/Commands/UICommandInfo.h"
#include "Framework/Notifications/NotificationManager.h"
#include "GameModes/UltraWorldSettings.h"
#include "InputCoreTypes.h"
#include "Internationalization/Internationalization.h"
#include "Misc/AssertionMacros.h"
#include "Misc/Optional.h"
#include "PlayInEditorDataTypes.h"
#include "Settings/ContentBrowserSettings.h"
#include "Settings/LevelEditorPlaySettings.h"
#include "Templates/Casts.h"
#include "Templates/SharedPointer.h"
#include "UObject/ObjectPtr.h"
#include "Widgets/Notifications/SNotificationList.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraEditorEngine)

class IEngineLoop;

#define LOCTEXT_NAMESPACE "UltraEditor"

UUltraEditorEngine::UUltraEditorEngine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUltraEditorEngine::Init(IEngineLoop* InEngineLoop)
{
	Super::Init(InEngineLoop);
}

void UUltraEditorEngine::Start()
{
	Super::Start();
}

void UUltraEditorEngine::Tick(float DeltaSeconds, bool bIdleMode)
{
	Super::Tick(DeltaSeconds, bIdleMode);
	
	FirstTickSetup();
}

void UUltraEditorEngine::FirstTickSetup()
{
	if (bFirstTickSetup)
	{
		return;
	}

	bFirstTickSetup = true;

	// Force show plugin content on load.
	GetMutableDefault<UContentBrowserSettings>()->SetDisplayPluginFolders(true);

}

FGameInstancePIEResult UUltraEditorEngine::PreCreatePIEInstances(const bool bAnyBlueprintErrors, const bool bStartInSpectatorMode, const float PIEStartTime, const bool bSupportsOnlinePIE, int32& InNumOnlinePIEInstances)
{
	if (const AUltraWorldSettings* UltraWorldSettings = Cast<AUltraWorldSettings>(EditorWorld->GetWorldSettings()))
	{
		if (UltraWorldSettings->ForceStandaloneNetMode)
		{
			EPlayNetMode OutPlayNetMode;
			PlaySessionRequest->EditorPlaySettings->GetPlayNetMode(OutPlayNetMode);
			if (OutPlayNetMode != PIE_Standalone)
			{
				PlaySessionRequest->EditorPlaySettings->SetPlayNetMode(PIE_Standalone);

				FNotificationInfo Info(LOCTEXT("ForcingStandaloneForFrontend", "Forcing NetMode: Standalone for the Frontend"));
				Info.ExpireDuration = 2.0f;
				FSlateNotificationManager::Get().AddNotification(Info);
			}
		}
	}

	//@TODO: Should add delegates that a *non-editor* module could bind to for PIE start/stop instead of poking directly
	GetDefault<UUltraDeveloperSettings>()->OnPlayInEditorStarted();
	GetDefault<UUltraPlatformEmulationSettings>()->OnPlayInEditorStarted();

	//
	FGameInstancePIEResult Result = Super::PreCreatePIEServerInstance(bAnyBlueprintErrors, bStartInSpectatorMode, PIEStartTime, bSupportsOnlinePIE, InNumOnlinePIEInstances);

	return Result;
}

#undef LOCTEXT_NAMESPACE