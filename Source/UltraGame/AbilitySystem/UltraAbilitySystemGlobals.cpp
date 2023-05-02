// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraAbilitySystemGlobals.h"

#include "UltraGameplayEffectContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraAbilitySystemGlobals)

struct FGameplayEffectContext;

UUltraAbilitySystemGlobals::UUltraAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FGameplayEffectContext* UUltraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FUltraGameplayEffectContext();
}

