// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraCharacterWithAbilities.h"

#include "AbilitySystem/Attributes/UltraCombatSet.h"
#include "AbilitySystem/Attributes/UltraHealthSet.h"
#include "AbilitySystem/UltraAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "HAL/Platform.h"
#include "Misc/AssertionMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraCharacterWithAbilities)

AUltraCharacterWithAbilities::AUltraCharacterWithAbilities(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UUltraAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	CreateDefaultSubobject<UUltraHealthSet>(TEXT("HealthSet"));
	CreateDefaultSubobject<UUltraCombatSet>(TEXT("CombatSet"));

	// AbilitySystemComponent needs to be updated at a high frequency.
	NetUpdateFrequency = 100.0f;
}

void AUltraCharacterWithAbilities::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

UAbilitySystemComponent* AUltraCharacterWithAbilities::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

