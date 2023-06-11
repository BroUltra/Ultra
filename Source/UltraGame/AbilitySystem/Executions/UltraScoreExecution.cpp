// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraScoreExecution.h"
#include "AbilitySystem/Attributes/UltraStyleSet.h"
#include "AbilitySystem/Attributes/UltraScoreSet.h"
#include "AbilitySystem/UltraGameplayEffectContext.h"
#include "AbilitySystem/UltraAbilitySourceInterface.h"
#include "Engine/World.h"
#include "Teams/UltraTeamSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraScoreExecution)

struct FScoreStatics
{
	FGameplayEffectAttributeCaptureDefinition BaseScoreDef;

	FScoreStatics()
	{
		BaseScoreDef = FGameplayEffectAttributeCaptureDefinition(UUltraScoreSet::GetBaseScoreAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static FScoreStatics& ScoreStatics()
{
	static FScoreStatics Statics;
	return Statics;
}


UUltraScoreExecution::UUltraScoreExecution()
{
	RelevantAttributesToCapture.Add(ScoreStatics().BaseScoreDef);
}

void UUltraScoreExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FUltraGameplayEffectContext* TypedContext = FUltraGameplayEffectContext::ExtractEffectContext(Spec.GetContext());
	check(TypedContext);

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float BaseScore = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ScoreStatics().BaseScoreDef, EvaluateParameters, BaseScore);

	const AActor* EffectCauser = TypedContext->GetEffectCauser();
	const FHitResult* HitActorResult = TypedContext->GetHitResult();

	AActor* HitActor = nullptr;
	FVector ImpactLocation = FVector::ZeroVector;
	FVector ImpactNormal = FVector::ZeroVector;
	FVector StartTrace = FVector::ZeroVector;
	FVector EndTrace = FVector::ZeroVector;

	// Calculation of hit actor, surface, zone, and distance all rely on whether the calculation has a hit result or not.
	// Effects just being added directly w/o having been targeted will always come in without a hit result, which must default
	// to some fallback information.
	if (HitActorResult)
	{
		const FHitResult& CurHitResult = *HitActorResult;
		HitActor = CurHitResult.HitObjectHandle.FetchActor();
		if (HitActor)
		{
			ImpactLocation = CurHitResult.ImpactPoint;
			ImpactNormal = CurHitResult.ImpactNormal;
			StartTrace = CurHitResult.TraceStart;
			EndTrace = CurHitResult.TraceEnd;
		}
	}

	// Handle case of no hit result or hit result not actually returning an actor
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	if (!HitActor)
	{
		HitActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor_Direct() : nullptr;
		if (HitActor)
		{
			ImpactLocation = HitActor->GetActorLocation();
		}
	}

	// Apply rules for team score/self score/etc...
	float ScoreInteractionAllowedMultiplier = 0.0f;
	if (HitActor)
	{
		UUltraTeamSubsystem* TeamSubsystem = HitActor->GetWorld()->GetSubsystem<UUltraTeamSubsystem>();
		if (ensure(TeamSubsystem))
		{
			ScoreInteractionAllowedMultiplier = TeamSubsystem->CanCauseScore(EffectCauser, HitActor) ? 1.0 : 0.0;
		}
	}

	// Determine distance
	double Distance = WORLD_MAX;

	if (TypedContext->HasOrigin())
	{
		Distance = FVector::Dist(TypedContext->GetOrigin(), ImpactLocation);
	}
	else if (EffectCauser)
	{
		Distance = FVector::Dist(EffectCauser->GetActorLocation(), ImpactLocation);
	}
	else
	{
		ensureMsgf(false, TEXT("Score Calculation cannot deduce a source location for score coming from %s; Falling back to WORLD_MAX dist!"), *GetPathNameSafe(Spec.Def));
	}

	// Apply ability source modifiers
	float PhysicalMaterialAttenuation = 1.0f;
	float DistanceAttenuation = 1.0f;
	if (const IUltraAbilitySourceInterface* AbilitySource = TypedContext->GetAbilitySource())
	{
		if (const UPhysicalMaterial* PhysMat = TypedContext->GetPhysicalMaterial())
		{
			PhysicalMaterialAttenuation = AbilitySource->GetPhysicalMaterialAttenuation(PhysMat, SourceTags, TargetTags);
		}

		DistanceAttenuation = AbilitySource->GetDistanceAttenuation(Distance, SourceTags, TargetTags);
	}
	DistanceAttenuation = FMath::Max(DistanceAttenuation, 0.0f);

	// Clamping is done when score is converted to -health
	const float ScoreDone = FMath::Max(BaseScore * DistanceAttenuation * PhysicalMaterialAttenuation * ScoreInteractionAllowedMultiplier, 0.0f);

	if (ScoreDone > 0.0f)
	{
		// Apply a score modifier, this gets turned into - health on the target
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UUltraStyleSet::GetScoreAttribute(), EGameplayModOp::Additive, ScoreDone));
	}
#endif // #if WITH_SERVER_CODE
}