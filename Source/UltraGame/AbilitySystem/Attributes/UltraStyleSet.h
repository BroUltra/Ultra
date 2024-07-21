// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "UltraAttributeSet.h"
#include "NativeGameplayTags.h"

#include "UltraStyleSet.generated.h"

class UObject;
struct FFrame;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Score);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_ScoreImmunity);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ultra_Score_Message);

struct FGameplayEffectModCallbackData;


/**
 * UUltraStyleSet
 *
 *	Class that defines attributes that are necessary for obtaining score.
 *	Attribute examples include: style, shields, and resistances.
 */
UCLASS(BlueprintType)
class UUltraStyleSet : public UUltraAttributeSet
{
	GENERATED_BODY()

public:

	UUltraStyleSet();

	ATTRIBUTE_ACCESSORS(UUltraStyleSet, Style);
	ATTRIBUTE_ACCESSORS(UUltraStyleSet, MaxStyle);
	ATTRIBUTE_ACCESSORS(UUltraStyleSet, Score);

protected:

	UFUNCTION()
	void OnRep_Style(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxStyle(const FGameplayAttributeData& OldValue);

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:

	// The current style attribute.  Style will be capped by max style.  Style is hidden from modifiers so only executions can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Style, Category = "Ultra|Style", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Style;

	// The max style value.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStyle, Category = "Ultra|Style", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStyle;
	
	// -------------------------------------------------------------------
	//	Meta Attribute (please keep attributes that aren't 'stateful' below 
	// -------------------------------------------------------------------

private:

	// Incoming score. This is mapped directly to +Style
	UPROPERTY(BlueprintReadOnly, Category="Ultra|Style", Meta=(HideFromModifiers, AllowPrivateAccess=true))
	FGameplayAttributeData Score;
};