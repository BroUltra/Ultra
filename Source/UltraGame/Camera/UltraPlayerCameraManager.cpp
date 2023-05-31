// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraPlayerCameraManager.h"

#include "Async/TaskGraphInterfaces.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "UltraCameraComponent.h"
#include "UltraUICameraManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraPlayerCameraManager)

class FDebugDisplayInfo;

static FName UICameraComponentName(TEXT("UICamera"));

AUltraPlayerCameraManager::AUltraPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultFOV = ULTRA_CAMERA_DEFAULT_FOV;
	ViewPitchMin = ULTRA_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = ULTRA_CAMERA_DEFAULT_PITCH_MAX;

	UICamera = CreateDefaultSubobject<UUltraUICameraManagerComponent>(UICameraComponentName);
}

UUltraUICameraManagerComponent* AUltraPlayerCameraManager::GetUICameraComponent() const
{
	return UICamera;
}

void AUltraPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	// If the UI Camera is looking at something, let it have priority.
	if (UICamera->NeedsToUpdateViewTarget())
	{
		Super::UpdateViewTarget(OutVT, DeltaTime);
		UICamera->UpdateViewTarget(OutVT, DeltaTime);
		return;
	}

	Super::UpdateViewTarget(OutVT, DeltaTime);
}

void AUltraPlayerCameraManager::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos)
{
	check(Canvas);

	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetFont(GEngine->GetSmallFont());
	DisplayDebugManager.SetDrawColor(FColor::Yellow);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("UltraPlayerCameraManager: %s"), *GetNameSafe(this)));

	Super::DisplayDebug(Canvas, DebugDisplay, YL, YPos);

	const APawn* Pawn = (PCOwner ? PCOwner->GetPawn() : nullptr);

	if (const UUltraCameraComponent* CameraComponent = UUltraCameraComponent::FindCameraComponent(Pawn))
	{
		CameraComponent->DrawDebug(Canvas);
	}
}

