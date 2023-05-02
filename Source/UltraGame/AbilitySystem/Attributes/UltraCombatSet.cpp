// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraCombatSet.h"

#include "Containers/Array.h"
#include "Net/UnrealNetwork.h"
#include "UObject/CoreNetTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraCombatSet)

class FLifetimeProperty;


UUltraCombatSet::UUltraCombatSet()
	: BaseDamage(0.0f)
	, BaseHeal(0.0f)
{
}

void UUltraCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UUltraCombatSet, BaseDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUltraCombatSet, BaseHeal, COND_OwnerOnly, REPNOTIFY_Always);
}

void UUltraCombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUltraCombatSet, BaseDamage, OldValue);
}

void UUltraCombatSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUltraCombatSet, BaseHeal, OldValue);
}

