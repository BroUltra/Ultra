// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "UObject/UObjectGlobals.h"

#include "UltraHealExecution.generated.h"

class UObject;


/**
 * UUltraHealExecution
 *
 *	Execution used by gameplay effects to apply healing to the health attributes.
 */
UCLASS()
class UUltraHealExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UUltraHealExecution();

protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
