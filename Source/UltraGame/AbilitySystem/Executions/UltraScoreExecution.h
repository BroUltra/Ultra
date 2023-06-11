// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayEffectExecutionCalculation.h"

#include "UltraScoreExecution.generated.h"

class UObject;


/**
 * UUltraScoreExecution
 *
 *	Execution used by gameplay effects to apply score to the health attributes.
 */
UCLASS()
class UUltraScoreExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UUltraScoreExecution();

protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};