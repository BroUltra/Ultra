// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraAbilityCost_InventoryItem.h"
#include "UltraGameplayAbility.h"
#include "Inventory/UltraInventoryManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraAbilityCost_InventoryItem)

UUltraAbilityCost_InventoryItem::UUltraAbilityCost_InventoryItem()
{
	Quantity.SetValue(1.0f);
}

bool UUltraAbilityCost_InventoryItem::CheckCost(const UUltraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
#if 0
	if (AController* PC = Ability->GetControllerFromActorInfo())
	{
		if (UUltraInventoryManagerComponent* InventoryComponent = PC->GetComponentByClass<UUltraInventoryManagerComponent>())
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

			const float NumItemsToConsumeReal = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumItemsToConsume = FMath::TruncToInt(NumItemsToConsumeReal);

			return InventoryComponent->GetTotalItemCountByDefinition(ItemDefinition) >= NumItemsToConsume;
		}
	}
#endif
	return false;
}

void UUltraAbilityCost_InventoryItem::ApplyCost(const UUltraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
#if 0
	if (ActorInfo->IsNetAuthority())
	{
		if (AController* PC = Ability->GetControllerFromActorInfo())
		{
			if (UUltraInventoryManagerComponent* InventoryComponent = PC->GetComponentByClass<UUltraInventoryManagerComponent>())
			{
				const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

				const float NumItemsToConsumeReal = Quantity.GetValueAtLevel(AbilityLevel);
				const int32 NumItemsToConsume = FMath::TruncToInt(NumItemsToConsumeReal);

				InventoryComponent->ConsumeItemsByDefinition(ItemDefinition, NumItemsToConsume);
			}
		}
	}
#endif
}

