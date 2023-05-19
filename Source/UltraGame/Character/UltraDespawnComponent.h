// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/GameFrameworkComponent.h"
#include "Delegates/Delegate.h"
#include "GameFramework/Actor.h"
#include "HAL/Platform.h"
#include "UObject/UObjectGlobals.h"

#include "UltraDespawnComponent.generated.h"

class UUltraAbilitySystemComponent;
class UObject;
struct FFrame;
struct FGameplayEffectSpec;
struct FOnAttributeChangeData;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUltraDespawn_DespawnEvent, AActor*, OwningActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FUltraDespawn_AttributeChanged, UUltraDespawnComponent*, DespawnComponent, float, OldValue, float, NewValue, AActor*, Instigator);


/**
 * EUltraDespawnState
 *
 *	Defines current state of despawn.
 */
UENUM(BlueprintType)
enum class EUltraDespawnState : uint8
{
	NotDespawned = 0,
	DespawnStarted,
	DespawnFinished
};


/**
 * UUltraDespawnComponent
 *
 *	An actor component used to handle anything related to despawning.
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class ULTRAGAME_API UUltraDespawnComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:

	UUltraDespawnComponent(const FObjectInitializer& ObjectInitializer);

	// Returns the despawn component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "Ultra|Despawn")
	static UUltraDespawnComponent* FindDespawnComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UUltraDespawnComponent>() : nullptr); }

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "Ultra|Despawn")
	void InitializeWithAbilitySystem(UUltraAbilitySystemComponent* InASC);

	// Uninitialize the component, clearing any references to the ability system.
	UFUNCTION(BlueprintCallable, Category = "Ultra|Despawn")
	void UninitializeFromAbilitySystem();

	UFUNCTION(BlueprintCallable, Category = "Ultra|Despawn")
	EUltraDespawnState GetDespawnState() const { return DespawnState; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Ultra|Despawn", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsDespawnedOrDespawning() const { return (DespawnState > EUltraDespawnState::NotDespawned); }

	// Begins the despawn sequence for the owner.
	virtual void StartDespawn();

	// Ends the despawn sequence for the owner.
	virtual void FinishDespawn();

public:

	// Delegate fired when the despawn sequence has started.
	UPROPERTY(BlueprintAssignable)
	FUltraDespawn_DespawnEvent OnDespawnStarted;

	// Delegate fired when the despawn sequence has finished.
	UPROPERTY(BlueprintAssignable)
	FUltraDespawn_DespawnEvent OnDespawnFinished;

protected:

	virtual void OnUnregister() override;

	void ClearGameplayTags();

	UFUNCTION()
	virtual void OnRep_DespawnState(EUltraDespawnState OldDespawnState);

protected:

	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<UUltraAbilitySystemComponent> AbilitySystemComponent;

	// Replicated state used to handle despawning.
	UPROPERTY(ReplicatedUsing = OnRep_DespawnState)
	EUltraDespawnState DespawnState;
};
