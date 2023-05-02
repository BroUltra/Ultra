// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraCheatManager.h"
#include "UltraPlayerController.h"
#include "UltraDebugCameraController.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "Engine/Console.h"
#include "GameFramework/HUD.h"
#include "System/UltraAssetManager.h"
#include "System/UltraGameData.h"
#include "UltraGameplayTags.h"
#include "AbilitySystem/UltraAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffect.h"
#include "Character/UltraHealthComponent.h"
#include "Character/UltraPawnExtensionComponent.h"
#include "System/UltraSystemStatics.h"
#include "Development/UltraDeveloperSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraCheatManager)

DEFINE_LOG_CATEGORY(LogUltraCheat);

namespace UltraCheat
{
	static const FName NAME_Fixed = FName(TEXT("Fixed"));
	
	static bool bEnableDebugCameraCycling = false;
	static FAutoConsoleVariableRef CVarEnableDebugCameraCycling(
		TEXT("UltraCheat.EnableDebugCameraCycling"),
		bEnableDebugCameraCycling,
		TEXT("If true then you can cycle the debug camera while running the game."),
		ECVF_Cheat);

	static bool bStartInGodMode = false;
	static FAutoConsoleVariableRef CVarStartInGodMode(
		TEXT("UltraCheat.StartInGodMode"),
		bStartInGodMode,
		TEXT("If true then the God cheat will be applied on begin play"),
		ECVF_Cheat);
};


UUltraCheatManager::UUltraCheatManager()
{
	DebugCameraControllerClass = AUltraDebugCameraController::StaticClass();
}

void UUltraCheatManager::InitCheatManager()
{
	Super::InitCheatManager();

#if WITH_EDITOR
	if (GIsEditor)
	{
		APlayerController* PC = GetOuterAPlayerController();
		for (const FUltraCheatToRun& CheatRow : GetDefault<UUltraDeveloperSettings>()->CheatsToRun)
		{
			if (CheatRow.Phase == ECheatExecutionTime::OnCheatManagerCreated)
			{
				PC->ConsoleCommand(CheatRow.Cheat, /*bWriteToLog=*/ true);
			}
		}
	}
#endif

	if (UltraCheat::bStartInGodMode)
	{
		God();	
	}
}

void UUltraCheatManager::CheatOutputText(const FString& TextToOutput)
{
#if USING_CHEAT_MANAGER
	// Output to the console.
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->ViewportConsole)
	{
		GEngine->GameViewport->ViewportConsole->OutputText(TextToOutput);
	}

	// Output to log.
	UE_LOG(LogUltraCheat, Display, TEXT("%s"), *TextToOutput);
#endif // USING_CHEAT_MANAGER
}

void UUltraCheatManager::Cheat(const FString& Msg)
{
	if (AUltraPlayerController* UltraPC = Cast<AUltraPlayerController>(GetOuterAPlayerController()))
	{
		UltraPC->ServerCheat(Msg.Left(128));
	}
}

void UUltraCheatManager::CheatAll(const FString& Msg)
{
	if (AUltraPlayerController* UltraPC = Cast<AUltraPlayerController>(GetOuterAPlayerController()))
	{
		UltraPC->ServerCheatAll(Msg.Left(128));
	}
}

void UUltraCheatManager::PlayNextGame()
{
	UUltraSystemStatics::PlayNextGame(this);
}

void UUltraCheatManager::EnableDebugCamera()
{
	Super::EnableDebugCamera();
}

void UUltraCheatManager::DisableDebugCamera()
{
	FVector DebugCameraLocation;
	FRotator DebugCameraRotation;

	ADebugCameraController* DebugCC = Cast<ADebugCameraController>(GetOuter());
	APlayerController* OriginalPC = nullptr;

	if (DebugCC)
	{
		OriginalPC = DebugCC->OriginalControllerRef;
		DebugCC->GetPlayerViewPoint(DebugCameraLocation, DebugCameraRotation);
	}

	Super::DisableDebugCamera();

	if (OriginalPC && OriginalPC->PlayerCameraManager && (OriginalPC->PlayerCameraManager->CameraStyle == UltraCheat::NAME_Fixed))
	{
		OriginalPC->SetInitialLocationAndRotation(DebugCameraLocation, DebugCameraRotation);

		OriginalPC->PlayerCameraManager->ViewTarget.POV.Location = DebugCameraLocation;
		OriginalPC->PlayerCameraManager->ViewTarget.POV.Rotation = DebugCameraRotation;
		OriginalPC->PlayerCameraManager->PendingViewTarget.POV.Location = DebugCameraLocation;
		OriginalPC->PlayerCameraManager->PendingViewTarget.POV.Rotation = DebugCameraRotation;
	}
}

bool UUltraCheatManager::InDebugCamera() const
{
	return (Cast<ADebugCameraController>(GetOuter()) ? true : false);
}

void UUltraCheatManager::EnableFixedCamera()
{
	const ADebugCameraController* DebugCC = Cast<ADebugCameraController>(GetOuter());
	APlayerController* PC = (DebugCC ? ToRawPtr(DebugCC->OriginalControllerRef) : GetOuterAPlayerController());

	if (PC && PC->PlayerCameraManager)
	{
		PC->SetCameraMode(UltraCheat::NAME_Fixed);
	}
}

void UUltraCheatManager::DisableFixedCamera()
{
	const ADebugCameraController* DebugCC = Cast<ADebugCameraController>(GetOuter());
	APlayerController* PC = (DebugCC ? ToRawPtr(DebugCC->OriginalControllerRef) : GetOuterAPlayerController());

	if (PC && PC->PlayerCameraManager)
	{
		PC->SetCameraMode(NAME_Default);
	}
}

bool UUltraCheatManager::InFixedCamera() const
{
	const ADebugCameraController* DebugCC = Cast<ADebugCameraController>(GetOuter());
	const APlayerController* PC = (DebugCC ? ToRawPtr(DebugCC->OriginalControllerRef) : GetOuterAPlayerController());

	if (PC && PC->PlayerCameraManager)
	{
		return (PC->PlayerCameraManager->CameraStyle == UltraCheat::NAME_Fixed);
	}

	return false;
}

void UUltraCheatManager::ToggleFixedCamera()
{
	if (InFixedCamera())
	{
		DisableFixedCamera();
	}
	else
	{
		EnableFixedCamera();
	}
}

void UUltraCheatManager::CycleDebugCameras()
{
	if (!UltraCheat::bEnableDebugCameraCycling)
	{
		return;
	}
	
	if (InDebugCamera())
	{
		EnableFixedCamera();
		DisableDebugCamera();
	}
	else if (InFixedCamera())
	{
		DisableFixedCamera();
		DisableDebugCamera();
	}
	else
	{
		EnableDebugCamera();
		DisableFixedCamera();
	}
}

void UUltraCheatManager::CycleAbilitySystemDebug()
{
	APlayerController* PC = Cast<APlayerController>(GetOuterAPlayerController());

	if (PC && PC->MyHUD)
	{
		if (!PC->MyHUD->bShowDebugInfo || !PC->MyHUD->DebugDisplay.Contains(TEXT("AbilitySystem")))
		{
			PC->MyHUD->ShowDebug(TEXT("AbilitySystem"));
		}

		PC->ConsoleCommand(TEXT("AbilitySystem.Debug.NextCategory"));
	}
}

void UUltraCheatManager::CancelActivatedAbilities()
{
	if (UUltraAbilitySystemComponent* UltraASC = GetPlayerAbilitySystemComponent())
	{
		const bool bReplicateCancelAbility = true;
		UltraASC->CancelInputActivatedAbilities(bReplicateCancelAbility);
	}
}

void UUltraCheatManager::AddTagToSelf(FString TagName)
{
	FGameplayTag Tag = FUltraGameplayTags::FindTagByString(TagName, true);
	if (Tag.IsValid())
	{
		if (UUltraAbilitySystemComponent* UltraASC = GetPlayerAbilitySystemComponent())
		{
			UltraASC->AddDynamicTagGameplayEffect(Tag);
		}
	}
	else
	{
		UE_LOG(LogUltraCheat, Display, TEXT("AddTagToSelf: Could not find any tag matching [%s]."), *TagName);
	}
}

void UUltraCheatManager::RemoveTagFromSelf(FString TagName)
{
	FGameplayTag Tag = FUltraGameplayTags::FindTagByString(TagName, true);
	if (Tag.IsValid())
	{
		if (UUltraAbilitySystemComponent* UltraASC = GetPlayerAbilitySystemComponent())
		{
			UltraASC->RemoveDynamicTagGameplayEffect(Tag);
		}
	}
	else
	{
		UE_LOG(LogUltraCheat, Display, TEXT("RemoveTagFromSelf: Could not find any tag matching [%s]."), *TagName);
	}
}

void UUltraCheatManager::DamageSelf(float DamageAmount)
{
	if (UUltraAbilitySystemComponent* UltraASC = GetPlayerAbilitySystemComponent())
	{
		ApplySetByCallerDamage(UltraASC, DamageAmount);
	}
}

void UUltraCheatManager::DamageTarget(float DamageAmount)
{
	if (AUltraPlayerController* UltraPC = Cast<AUltraPlayerController>(GetOuterAPlayerController()))
	{
		if (UltraPC->GetNetMode() == NM_Client)
		{
			// Automatically send cheat to server for convenience.
			UltraPC->ServerCheat(FString::Printf(TEXT("DamageTarget %.2f"), DamageAmount));
			return;
		}

		FHitResult TargetHitResult;
		AActor* TargetActor = GetTarget(UltraPC, TargetHitResult);

		if (UUltraAbilitySystemComponent* UltraTargetASC = Cast<UUltraAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor)))
		{
			ApplySetByCallerDamage(UltraTargetASC, DamageAmount);
		}
	}
}

void UUltraCheatManager::ApplySetByCallerDamage(UUltraAbilitySystemComponent* UltraASC, float DamageAmount)
{
	check(UltraASC);

	TSubclassOf<UGameplayEffect> DamageGE = UUltraAssetManager::GetSubclass(UUltraGameData::Get().DamageGameplayEffect_SetByCaller);
	FGameplayEffectSpecHandle SpecHandle = UltraASC->MakeOutgoingSpec(DamageGE, 1.0f, UltraASC->MakeEffectContext());

	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(FUltraGameplayTags::Get().SetByCaller_Damage, DamageAmount);
		UltraASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void UUltraCheatManager::HealSelf(float HealAmount)
{
	if (UUltraAbilitySystemComponent* UltraASC = GetPlayerAbilitySystemComponent())
	{
		ApplySetByCallerHeal(UltraASC, HealAmount);
	}
}

void UUltraCheatManager::HealTarget(float HealAmount)
{
	if (AUltraPlayerController* UltraPC = Cast<AUltraPlayerController>(GetOuterAPlayerController()))
	{
		FHitResult TargetHitResult;
		AActor* TargetActor = GetTarget(UltraPC, TargetHitResult);

		if (UUltraAbilitySystemComponent* UltraTargetASC = Cast<UUltraAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor)))
		{
			ApplySetByCallerHeal(UltraTargetASC, HealAmount);
		}
	}
}

void UUltraCheatManager::ApplySetByCallerHeal(UUltraAbilitySystemComponent* UltraASC, float HealAmount)
{
	check(UltraASC);

	TSubclassOf<UGameplayEffect> HealGE = UUltraAssetManager::GetSubclass(UUltraGameData::Get().HealGameplayEffect_SetByCaller);
	FGameplayEffectSpecHandle SpecHandle = UltraASC->MakeOutgoingSpec(HealGE, 1.0f, UltraASC->MakeEffectContext());

	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(FUltraGameplayTags::Get().SetByCaller_Heal, HealAmount);
		UltraASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

UUltraAbilitySystemComponent* UUltraCheatManager::GetPlayerAbilitySystemComponent() const
{
	if (AUltraPlayerController* UltraPC = Cast<AUltraPlayerController>(GetOuterAPlayerController()))
	{
		return UltraPC->GetUltraAbilitySystemComponent();
	}
	return nullptr;
}

void UUltraCheatManager::DamageSelfDestruct()
{
	if (AUltraPlayerController* UltraPC = Cast<AUltraPlayerController>(GetOuterAPlayerController()))
	{
 		if (const UUltraPawnExtensionComponent* PawnExtComp = UUltraPawnExtensionComponent::FindPawnExtensionComponent(UltraPC->GetPawn()))
		{
			if (PawnExtComp->HasReachedInitState(FUltraGameplayTags::Get().InitState_GameplayReady))
			{
				if (UUltraHealthComponent* HealthComponent = UUltraHealthComponent::FindHealthComponent(UltraPC->GetPawn()))
				{
					HealthComponent->DamageSelfDestruct();
				}
			}
		}
	}
}

void UUltraCheatManager::God()
{
	if (AUltraPlayerController* UltraPC = Cast<AUltraPlayerController>(GetOuterAPlayerController()))
	{
		if (UltraPC->GetNetMode() == NM_Client)
		{
			// Automatically send cheat to server for convenience.
			UltraPC->ServerCheat(FString::Printf(TEXT("God")));
			return;
		}

		if (UUltraAbilitySystemComponent* UltraASC = UltraPC->GetUltraAbilitySystemComponent())
		{
			const FGameplayTag Tag = FUltraGameplayTags::Get().Cheat_GodMode;
			const bool bHasTag = UltraASC->HasMatchingGameplayTag(Tag);

			if (bHasTag)
			{
				UltraASC->RemoveDynamicTagGameplayEffect(Tag);
			}
			else
			{
				UltraASC->AddDynamicTagGameplayEffect(Tag);
			}
		}
	}
}

void UUltraCheatManager::UnlimitedHealth(int32 Enabled)
{
	if (UUltraAbilitySystemComponent* UltraASC = GetPlayerAbilitySystemComponent())
	{
		const FGameplayTag Tag = FUltraGameplayTags::Get().Cheat_UnlimitedHealth;
		const bool bHasTag = UltraASC->HasMatchingGameplayTag(Tag);

		if ((Enabled == -1) || ((Enabled > 0) && !bHasTag) || ((Enabled == 0) && bHasTag))
		{
			if (bHasTag)
			{
				UltraASC->RemoveDynamicTagGameplayEffect(Tag);
			}
			else
			{
				UltraASC->AddDynamicTagGameplayEffect(Tag);
			}
		}
	}
}

