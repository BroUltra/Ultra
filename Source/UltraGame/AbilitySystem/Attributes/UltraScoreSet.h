// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "UltraAttributeSet.h"

#include "UltraScoreSet.generated.h"

class UObject;
struct FFrame;


/**
 * UUltraScoreSet
 *
 *  Class that defines attributes that are necessary for applying score.
 *	Attribute examples include: score and context-specific score value modifiers.
 */
UCLASS(BlueprintType)
class UUltraScoreSet : public UUltraAttributeSet
{
	GENERATED_BODY()

public:

	UUltraScoreSet();

	ATTRIBUTE_ACCESSORS(UUltraScoreSet, BaseScore);

protected:

	UFUNCTION()
	void OnRep_BaseScore(const FGameplayAttributeData& OldValue);

private:

	// The base amount of score to apply in the score execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseScore, Category = "Ultra|Score", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseScore;
};