// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraGameplayEffectContext.h"

#include "AbilitySystem/UltraAbilitySourceInterface.h"
#include "Engine/HitResult.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Templates/Casts.h"
#include "UObject/Object.h"

#if UE_WITH_IRIS
#include "Iris/ReplicationState/PropertyNetSerializerInfoRegistry.h"
#include "Serialization/GameplayEffectContextNetSerializer.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraGameplayEffectContext)

class FArchive;

FUltraGameplayEffectContext* FUltraGameplayEffectContext::ExtractEffectContext(struct FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FUltraGameplayEffectContext::StaticStruct()))
	{
		return (FUltraGameplayEffectContext*)BaseEffectContext;
	}

	return nullptr;
}

bool FUltraGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

	// Not serialized for post-activation use:
	// CartridgeID

	return true;
}

#if UE_WITH_IRIS
namespace UE::Net
{
	// Forward to FGameplayEffectContextNetSerializer
	// Note: If FUltraGameplayEffectContext::NetSerialize() is modified, a custom NetSerializesr must be implemented as the current fallback will no longer be sufficient.
	UE_NET_IMPLEMENT_FORWARDING_NETSERIALIZER_AND_REGISTRY_DELEGATES(UltraGameplayEffectContext, FGameplayEffectContextNetSerializer);
}
#endif

void FUltraGameplayEffectContext::SetAbilitySource(const IUltraAbilitySourceInterface* InObject, float InSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
	//SourceLevel = InSourceLevel;
}

const IUltraAbilitySourceInterface* FUltraGameplayEffectContext::GetAbilitySource() const
{
	return Cast<IUltraAbilitySourceInterface>(AbilitySourceObject.Get());
}

const UPhysicalMaterial* FUltraGameplayEffectContext::GetPhysicalMaterial() const
{
	if (const FHitResult* HitResultPtr = GetHitResult())
	{
		return HitResultPtr->PhysMaterial.Get();
	}
	return nullptr;
}

