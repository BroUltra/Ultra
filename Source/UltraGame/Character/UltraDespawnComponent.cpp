// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/UltraDespawnComponent.h"

#include "UltraLogChannels.h"
#include "System/UltraAssetManager.h"
#include "System/UltraGameData.h"
#include "UltraGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/UltraAbilitySystemComponent.h"
#include "Messages/UltraVerbMessage.h"
#include "Messages/UltraVerbMessageHelpers.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraDespawnComponent)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Ultra_Elimination_Message, "Ultra.Elimination.Message");


UUltraDespawnComponent::UUltraDespawnComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
	DespawnState = EUltraDespawnState::NotDespawned;
}

void UUltraDespawnComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UUltraDespawnComponent, DespawnState);
}

void UUltraDespawnComponent::OnUnregister()
{
	UninitializeFromAbilitySystem();

	Super::OnUnregister();
}

void UUltraDespawnComponent::InitializeWithAbilitySystem(UUltraAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogUltra, Error, TEXT("UltraDespawnComponent: Despawn component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogUltra, Error, TEXT("UltraDespawnComponent: Cannot initialize despawn component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
		return;
	}

	ClearGameplayTags();
}

void UUltraDespawnComponent::UninitializeFromAbilitySystem()
{
	ClearGameplayTags();
	
	AbilitySystemComponent = nullptr;
}

void UUltraDespawnComponent::ClearGameplayTags()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(UltraGameplayTags::Status_Despawn_Despawning, 0);
		AbilitySystemComponent->SetLooseGameplayTagCount(UltraGameplayTags::Status_Despawn_Despawned, 0);
	}
}

static AActor* GetInstigatorFromAttrChangeData(const FOnAttributeChangeData& ChangeData)
{
	if (ChangeData.GEModData != nullptr)
	{
		const FGameplayEffectContextHandle& EffectContext = ChangeData.GEModData->EffectSpec.GetEffectContext();
		return EffectContext.GetOriginalInstigator();
	}

    return nullptr;
}

void UUltraDespawnComponent::OnRep_DespawnState(EUltraDespawnState OldDespawnState)
{
	const EUltraDespawnState NewDespawnState = DespawnState;

	// Revert the despawn state for now since we rely on StartDespawn and FinishDespawn to change it.
	DespawnState = OldDespawnState;

	if (OldDespawnState > NewDespawnState)
	{
		// The server is trying to set us back but we've already predicted past the server state.
		UE_LOG(LogUltra, Warning, TEXT("UltraDespawnComponent: Predicted past server despawn state [%d] -> [%d] for owner [%s]."), (uint8)OldDespawnState, (uint8)NewDespawnState, *GetNameSafe(GetOwner()));
		return;
	}

	if (OldDespawnState == EUltraDespawnState::NotDespawned)
	{
		if (NewDespawnState == EUltraDespawnState::DespawnStarted)
		{
			StartDespawn();
		}
		else if (NewDespawnState == EUltraDespawnState::DespawnFinished)
		{
			StartDespawn();
			FinishDespawn();
		}
		else
		{
			UE_LOG(LogUltra, Error, TEXT("UltraDespawnComponent: Invalid despawn transition [%d] -> [%d] for owner [%s]."), (uint8)OldDespawnState, (uint8)NewDespawnState, *GetNameSafe(GetOwner()));
		}
	}
	else if (OldDespawnState == EUltraDespawnState::DespawnStarted)
	{
		if (NewDespawnState == EUltraDespawnState::DespawnFinished)
		{
			FinishDespawn();
		}
		else
		{
			UE_LOG(LogUltra, Error, TEXT("UltraDespawnComponent: Invalid despawn transition [%d] -> [%d] for owner [%s]."), (uint8)OldDespawnState, (uint8)NewDespawnState, *GetNameSafe(GetOwner()));
		}
	}

	ensureMsgf((DespawnState == NewDespawnState), TEXT("UltraDespawnComponent: Despawn transition failed [%d] -> [%d] for owner [%s]."), (uint8)OldDespawnState, (uint8)NewDespawnState, *GetNameSafe(GetOwner()));
}

void UUltraDespawnComponent::StartDespawn()
{
	if (DespawnState != EUltraDespawnState::NotDespawned)
	{
		return;
	}

	DespawnState = EUltraDespawnState::DespawnStarted;

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(UltraGameplayTags::Status_Despawn_Despawning, 1);
	}

	AActor* Owner = GetOwner();
	check(Owner);

	OnDespawnStarted.Broadcast(Owner);

	Owner->ForceNetUpdate();
}

void UUltraDespawnComponent::FinishDespawn()
{
	if (DespawnState != EUltraDespawnState::DespawnStarted)
	{
		return;
	}

	DespawnState = EUltraDespawnState::DespawnFinished;

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(UltraGameplayTags::Status_Despawn_Despawned, 1);
	}

	AActor* Owner = GetOwner();
	check(Owner);

	OnDespawnFinished.Broadcast(Owner);

	Owner->ForceNetUpdate();
}