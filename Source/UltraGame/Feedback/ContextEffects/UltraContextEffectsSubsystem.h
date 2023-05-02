// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Chaos/ChaosEngineInterface.h"
#include "Containers/Array.h"
#include "Containers/EnumAsByte.h"
#include "Containers/Map.h"
#include "Containers/Set.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "Math/MathFwd.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include "Subsystems/WorldSubsystem.h"
#include "UObject/NameTypes.h"
#include "UObject/Object.h"
#include "UObject/SoftObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "UltraContextEffectsSubsystem.generated.h"

class AActor;
class UAudioComponent;
class UUltraContextEffectsLibrary;
class UNiagaraComponent;
class USceneComponent;
struct FFrame;
struct FGameplayTag;
struct FGameplayTagContainer;

/**
 *
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "UltraContextEffects"))
class ULTRAGAME_API UUltraContextEffectsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	//
	UPROPERTY(config, EditAnywhere)
	TMap<TEnumAsByte<EPhysicalSurface>, FGameplayTag> SurfaceTypeToContextMap;
};

/**
 *
 */
UCLASS()
class ULTRAGAME_API UUltraContextEffectsSet : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	TSet<TObjectPtr<UUltraContextEffectsLibrary>> UltraContextEffectsLibraries;
};


/**
 * 
 */
UCLASS()
class ULTRAGAME_API UUltraContextEffectsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	/** */
	UFUNCTION(BlueprintCallable, Category = "ContextEffects")
	void SpawnContextEffects(
		const AActor* SpawningActor
		, USceneComponent* AttachToComponent
		, const FName AttachPoint
		, const FVector LocationOffset
		, const FRotator RotationOffset
		, FGameplayTag Effect
		, FGameplayTagContainer Contexts
		, TArray<UAudioComponent*>& AudioOut
		, TArray<UNiagaraComponent*>& NiagaraOut
		, FVector VFXScale = FVector(1)
		, float AudioVolume = 1
		, float AudioPitch = 1);

	/** */
	UFUNCTION(BlueprintCallable, Category = "ContextEffects")
	bool GetContextFromSurfaceType(TEnumAsByte<EPhysicalSurface> PhysicalSurface, FGameplayTag& Context);

	/** */
	UFUNCTION(BlueprintCallable, Category = "ContextEffects")
	void LoadAndAddContextEffectsLibraries(AActor* OwningActor, TSet<TSoftObjectPtr<UUltraContextEffectsLibrary>> ContextEffectsLibraries);

	/** */
	UFUNCTION(BlueprintCallable, Category = "ContextEffects")
	void UnloadAndRemoveContextEffectsLibraries(AActor* OwningActor);

private:

	UPROPERTY(Transient)
	TMap<TObjectPtr<AActor>, TObjectPtr<UUltraContextEffectsSet>> ActiveActorEffectsMap;

};