// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "UltraAbilityCost.h"
#include "ScalableFloat.h"
#include "UObject/UObjectGlobals.h"

#include "UltraAbilityCost_PlayerTagStack.generated.h"

class UUltraGameplayAbility;
class UObject;
struct FGameplayAbilityActorInfo;

/**
 * Represents a cost that requires expending a quantity of a tag stack on the player state
 */
UCLASS(meta=(DisplayName="Player Tag Stack"))
class UUltraAbilityCost_PlayerTagStack : public UUltraAbilityCost
{
	GENERATED_BODY()

public:
	UUltraAbilityCost_PlayerTagStack();

	//~UUltraAbilityCost interface
	virtual bool CheckCost(const UUltraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const UUltraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~End of UUltraAbilityCost interface

protected:
	/** How much of the tag to spend (keyed on ability level) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	FScalableFloat Quantity;

	/** Which tag to spend some of */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	FGameplayTag Tag;
};
