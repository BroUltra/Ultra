// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraStyleSet.h"
#include "AbilitySystem/Attributes/UltraAttributeSet.h"
#include "UltraGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/UltraAbilitySystemComponent.h"
#include "Engine/World.h"
#include "GameplayEffectExtension.h"
#include "Messages/UltraVerbMessage.h"
#include "GameFramework/GameplayMessageSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraStyleSet)

UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Score, "Gameplay.Score");
UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_ScoreImmunity, "Gameplay.ScoreImmunity");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ultra_Score_Message, "Ultra.Score.Message");

UUltraStyleSet::UUltraStyleSet()
	: Style(0.0f)
	, MaxStyle(999999999.0f)
{
}

void UUltraStyleSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UUltraStyleSet, Style, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UUltraStyleSet, MaxStyle, COND_None, REPNOTIFY_Always);
}

void UUltraStyleSet::OnRep_Style(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUltraStyleSet, Style, OldValue);
}

void UUltraStyleSet::OnRep_MaxStyle(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UUltraStyleSet, MaxStyle, OldValue);
}

bool UUltraStyleSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData &Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	/*
	// Handle modifying incoming normal score
	if (Data.EvaluatedData.Attribute == GetScoreAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			if (Data.Target.HasMatchingGameplayTag()
			{
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}
		}
	}
	*/

	return true;
}

void UUltraStyleSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	float MinimumStyle = 0.0f;

	if (Data.EvaluatedData.Attribute == GetScoreAttribute())
	{
		// Send a standardized verb message that other systems can observe
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			FUltraVerbMessage Message;
			Message.Verb = TAG_Ultra_Score_Message;
			Message.Instigator = Data.EffectSpec.GetEffectContext().GetEffectCauser();
			Message.InstigatorTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
			Message.Target = GetOwningActor();
			Message.TargetTags = *Data.EffectSpec.CapturedTargetTags.GetAggregatedTags();
			//@TODO: Fill out context tags, and any non-ability-system source/instigator tags
			Message.Magnitude = Data.EvaluatedData.Magnitude;

			UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
			MessageSystem.BroadcastMessage(Message.Verb, Message);
		}
		
		// Convert into +Style and then clamp
		SetStyle(FMath::Clamp(GetStyle() - GetScore(), MinimumStyle, GetMaxStyle()));
		SetScore(0.0f);
	}
}

void UUltraStyleSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UUltraStyleSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UUltraStyleSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxStyleAttribute())
	{
		// Make sure current style is not greater than the max style.
		if (GetStyle() > NewValue)
		{
			UUltraAbilitySystemComponent* UltraASC = GetUltraAbilitySystemComponent();
			check(UltraASC);

			UltraASC->ApplyModToAttribute(GetStyleAttribute(), EGameplayModOp::Override, NewValue);
		}
	}
}

void UUltraStyleSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetStyleAttribute())
	{
		// Do not allow style to go negative or above max style.
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStyle());
	}
}