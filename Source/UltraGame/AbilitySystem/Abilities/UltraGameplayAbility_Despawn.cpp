// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraGameplayAbility_Despawn.h"

#include "AbilitySystem/Abilities/UltraGameplayAbility.h"
#include "AbilitySystem/UltraAbilitySystemComponent.h"
#include "Character/UltraDespawnComponent.h"
#include "UltraGameplayTags.h"
#include "UltraLogChannels.h"
#include "Trace/Trace.inl"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraGameplayAbility_Despawn)

UUltraGameplayAbility_Despawn::UUltraGameplayAbility_Despawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	bAutoStartDespawn = true;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = UltraGameplayTags::GameplayEvent_Despawn;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UUltraGameplayAbility_Despawn::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	check(ActorInfo);

	UUltraAbilitySystemComponent* UltraASC = CastChecked<UUltraAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());

	FGameplayTagContainer AbilityTypesToIgnore;
	AbilityTypesToIgnore.AddTag(UltraGameplayTags::Ability_Behavior_SurvivesDespawn);

	// Cancel all abilities and block others from starting.
	UltraASC->CancelAbilities(nullptr, &AbilityTypesToIgnore, this);

	SetCanBeCanceled(false);

	if (!ChangeActivationGroup(EUltraAbilityActivationGroup::Exclusive_Blocking))
	{
		UE_LOG(LogUltraAbilitySystem, Error, TEXT("UUltraGameplayAbility_Despawn::ActivateAbility: Ability [%s] failed to change activation group to blocking."), *GetName());
	}

	if (bAutoStartDespawn)
	{
		StartDespawn();
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UUltraGameplayAbility_Despawn::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	check(ActorInfo);

	// Always try to finish the despawn when the ability ends in case the ability doesn't.
	// This won't do anything if the despawn hasn't been started.
	FinishDespawn();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UUltraGameplayAbility_Despawn::StartDespawn()
{
	if (UUltraDespawnComponent* DespawnComponent = UUltraDespawnComponent::FindDespawnComponent(GetAvatarActorFromActorInfo()))
	{
		if (DespawnComponent->GetDespawnState() == EUltraDespawnState::NotDespawned)
		{
			DespawnComponent->StartDespawn();
		}
	}
}

void UUltraGameplayAbility_Despawn::FinishDespawn()
{
	if (UUltraDespawnComponent* DespawnComponent = UUltraDespawnComponent::FindDespawnComponent(GetAvatarActorFromActorInfo()))
	{
		if (DespawnComponent->GetDespawnState() == EUltraDespawnState::DespawnStarted)
		{
			DespawnComponent->FinishDespawn();
		}
	}
}