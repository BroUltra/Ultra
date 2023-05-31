// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UltraGameplayAbility.h"

#include "UltraGameplayAbility_Despawn.generated.h"

class UObject;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayEventData;


/**
 * UUltraGameplayAbility_Despawn
 *
 *	Gameplay ability used for handling despawn.
 *	Ability is activated automatically via the "GameplayEvent.Despawn" ability trigger tag.
 */
UCLASS(Abstract)
class UUltraGameplayAbility_Despawn : public UUltraGameplayAbility
{
	GENERATED_BODY()

public:

	UUltraGameplayAbility_Despawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	// Starts the despawn sequence.
	UFUNCTION(BlueprintCallable, Category = "Ultra|Ability")
	void StartDespawn();

	// Finishes the despawn sequence.
	UFUNCTION(BlueprintCallable, Category = "Ultra|Ability")
	void FinishDespawn();

protected:

	// If enabled, the ability will automatically call StartDespawn.  FinishDespawn is always called when the ability ends if the despawn was started.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultra|Despawn")
	bool bAutoStartDespawn;
};
