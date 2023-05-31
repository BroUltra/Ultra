// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/UltraGameplayAbility_Reset.h"

#include "AbilitySystem/UltraAbilitySystemComponent.h"
#include "Character/UltraCharacter.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "UltraGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraGameplayAbility_Reset)

UUltraGameplayAbility_Reset::UUltraGameplayAbility_Reset(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = UltraGameplayTags::GameplayEvent_RequestReset;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UUltraGameplayAbility_Reset::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	check(ActorInfo);

	UUltraAbilitySystemComponent* UltraASC = CastChecked<UUltraAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());

	FGameplayTagContainer AbilityTypesToIgnore;
	AbilityTypesToIgnore.AddTag(UltraGameplayTags::Ability_Behavior_SurvivesDespawn);

	// Cancel all abilities and block others from starting.
	UltraASC->CancelAbilities(nullptr, &AbilityTypesToIgnore, this);
	
	SetCanBeCanceled(false);

	// Execute the reset from the character
	if (AUltraCharacter* UltraChar = Cast<AUltraCharacter>(CurrentActorInfo->AvatarActor.Get()))
	{
		UltraChar->Reset();
	}

	// Let others know a reset has occurred
	FUltraPlayerResetMessage Message;
	Message.OwnerPlayerState = CurrentActorInfo->OwnerActor.Get();
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
	MessageSystem.BroadcastMessage(UltraGameplayTags::GameplayEvent_Reset, Message);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bReplicateEndAbility = true;
	const bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);
}

