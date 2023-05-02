// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraUICameraManagerComponent.h"

#include "Delegates/Delegate.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "UltraPlayerCameraManager.h"
#include "Misc/CoreMisc.h"
#include "Templates/Casts.h"
#include "Templates/UnrealTemplate.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraUICameraManagerComponent)

class AActor;
class FDebugDisplayInfo;

UUltraUICameraManagerComponent* UUltraUICameraManagerComponent::GetComponent(APlayerController* PC)
{
	if (PC != nullptr)
	{
		if (AUltraPlayerCameraManager* PCCamera = Cast<AUltraPlayerCameraManager>(PC->PlayerCameraManager))
		{
			return PCCamera->GetUICameraComponent();
		}
	}

	return nullptr;
}

UUltraUICameraManagerComponent::UUltraUICameraManagerComponent()
{
	bWantsInitializeComponent = true;

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		// Register "showdebug" hook.
		if (!IsRunningDedicatedServer())
		{
			AHUD::OnShowDebugInfo.AddUObject(this, &ThisClass::OnShowDebugInfo);
		}
	}
}

void UUltraUICameraManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UUltraUICameraManagerComponent::SetViewTarget(AActor* InViewTarget, FViewTargetTransitionParams TransitionParams)
{
	TGuardValue<bool> UpdatingViewTargetGuard(bUpdatingViewTarget, true);

	ViewTarget = InViewTarget;
	CastChecked<AUltraPlayerCameraManager>(GetOwner())->SetViewTarget(ViewTarget, TransitionParams);
}

bool UUltraUICameraManagerComponent::NeedsToUpdateViewTarget() const
{
	return false;
}

void UUltraUICameraManagerComponent::UpdateViewTarget(struct FTViewTarget& OutVT, float DeltaTime)
{
}

void UUltraUICameraManagerComponent::OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos)
{
}
