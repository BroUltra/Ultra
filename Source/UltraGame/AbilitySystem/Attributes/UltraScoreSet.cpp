// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraScoreSet.h"

#include "AbilitySystem/Attributes/UltraAttributeSet.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraScoreSet)

class FLifetimeProperty;


UUltraScoreSet::UUltraScoreSet()
	: BaseScore(0.0f)
{
}

void UUltraScoreSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UUltraScoreSet, BaseScore, COND_OwnerOnly, REPNOTIFY_Always);
}

void UUltraScoreSet::OnRep_BaseScore(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUltraScoreSet, BaseScore, OldValue);
}