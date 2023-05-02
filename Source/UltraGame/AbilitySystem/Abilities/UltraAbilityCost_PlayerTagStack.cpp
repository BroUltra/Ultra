// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraAbilityCost_PlayerTagStack.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "GameFramework/Controller.h"
#include "UltraGameplayAbility.h"
#include "Math/UnrealMathSSE.h"
#include "Player/UltraPlayerState.h"
#include "Templates/Casts.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraAbilityCost_PlayerTagStack)

UUltraAbilityCost_PlayerTagStack::UUltraAbilityCost_PlayerTagStack()
{
	Quantity.SetValue(1.0f);
}

bool UUltraAbilityCost_PlayerTagStack::CheckCost(const UUltraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (AController* PC = Ability->GetControllerFromActorInfo())
	{
		if (AUltraPlayerState* PS = Cast<AUltraPlayerState>(PC->PlayerState))
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

			const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumStacks = FMath::TruncToInt(NumStacksReal);

			return PS->GetStatTagStackCount(Tag) >= NumStacks;
		}
	}
	return false;
}

void UUltraAbilityCost_PlayerTagStack::ApplyCost(const UUltraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo->IsNetAuthority())
	{
		if (AController* PC = Ability->GetControllerFromActorInfo())
		{
			if (AUltraPlayerState* PS = Cast<AUltraPlayerState>(PC->PlayerState))
			{
				const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

				const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
				const int32 NumStacks = FMath::TruncToInt(NumStacksReal);

				PS->RemoveStatTagStack(Tag, NumStacks);
			}
		}
	}
}

