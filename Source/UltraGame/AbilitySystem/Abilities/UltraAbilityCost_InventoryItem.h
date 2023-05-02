// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayAbilitySpec.h"
#include "UltraAbilityCost.h"
#include "ScalableFloat.h"
#include "Templates/SubclassOf.h"
#include "UObject/UObjectGlobals.h"

#include "UltraAbilityCost_InventoryItem.generated.h"

class UUltraGameplayAbility;
class UUltraInventoryItemDefinition;
class UObject;
struct FGameplayAbilityActorInfo;
struct FGameplayTagContainer;

/**
 * Represents a cost that requires expending a quantity of an inventory item
 */
UCLASS(meta=(DisplayName="Inventory Item"))
class UUltraAbilityCost_InventoryItem : public UUltraAbilityCost
{
	GENERATED_BODY()

public:
	UUltraAbilityCost_InventoryItem();

	//~UUltraAbilityCost interface
	virtual bool CheckCost(const UUltraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const UUltraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~End of UUltraAbilityCost interface

protected:
	/** How much of the item to spend (keyed on ability level) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AbilityCost)
	FScalableFloat Quantity;

	/** Which item to consume */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AbilityCost)
	TSubclassOf<UUltraInventoryItemDefinition> ItemDefinition;
};
