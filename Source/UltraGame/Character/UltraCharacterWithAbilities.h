// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Character/UltraCharacter.h"

#include "UltraCharacterWithAbilities.generated.h"

class UAbilitySystemComponent;
class UUltraAbilitySystemComponent;
class UObject;

// AUltraCharacter typically gets the ability system component from the possessing player state
// This represents a character with a self-contained ability system component.
UCLASS(Blueprintable)
class ULTRAGAME_API AUltraCharacterWithAbilities : public AUltraCharacter
{
	GENERATED_BODY()

public:
	AUltraCharacterWithAbilities(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:

	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Ultra|PlayerState")
	TObjectPtr<UUltraAbilitySystemComponent> AbilitySystemComponent;
};
