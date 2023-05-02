// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "UObject/UObjectGlobals.h"

#include "UltraDamageExecution.generated.h"

class UObject;


/**
 * UUltraDamageExecution
 *
 *	Execution used by gameplay effects to apply damage to the health attributes.
 */
UCLASS()
class UUltraDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UUltraDamageExecution();

protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
