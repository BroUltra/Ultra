// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraGameplayAbility_Jump.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "Character/UltraCharacter.h"
#include "Containers/EnumAsByte.h"
#include "GameFramework/Actor.h"
#include "Templates/Casts.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraGameplayAbility_Jump)

struct FGameplayTagContainer;


UUltraGameplayAbility_Jump::UUltraGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UUltraGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	const AUltraCharacter* UltraCharacter = Cast<AUltraCharacter>(ActorInfo->AvatarActor.Get());
	if (!UltraCharacter || !UltraCharacter->CanJump())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UUltraGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Stop jumping in case the ability blueprint doesn't call it.
	CharacterJumpStop();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UUltraGameplayAbility_Jump::CharacterJumpStart()
{
	if (AUltraCharacter* UltraCharacter = GetUltraCharacterFromActorInfo())
	{
		if (UltraCharacter->IsLocallyControlled() && !UltraCharacter->bPressedJump)
		{
			UltraCharacter->UnCrouch();
			UltraCharacter->Jump();
		}
	}
}

void UUltraGameplayAbility_Jump::CharacterJumpStop()
{
	if (AUltraCharacter* UltraCharacter = GetUltraCharacterFromActorInfo())
	{
		if (UltraCharacter->IsLocallyControlled() && UltraCharacter->bPressedJump)
		{
			UltraCharacter->StopJumping();
		}
	}
}

