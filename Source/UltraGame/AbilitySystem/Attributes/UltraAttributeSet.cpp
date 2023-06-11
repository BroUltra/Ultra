// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraAttributeSet.h"

#include "AbilitySystem/UltraAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraAttributeSet)

class UWorld;


UUltraAttributeSet::UUltraAttributeSet()
{
}

UWorld* UUltraAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UUltraAbilitySystemComponent* UUltraAttributeSet::GetUltraAbilitySystemComponent() const
{
	return Cast<UUltraAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}