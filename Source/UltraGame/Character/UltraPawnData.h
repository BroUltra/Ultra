// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/Array.h"
#include "Engine/DataAsset.h"
#include "Templates/SubclassOf.h"
#include "UObject/UObjectGlobals.h"

#include "UltraPawnData.generated.h"

class APawn;
class UUltraAbilitySet;
class UUltraAbilityTagRelationshipMapping;
class UUltraCameraMode;
class UUltraInputConfig;
class UObject;


/**
 * UUltraPawnData
 *
 *	Non-mutable data asset that contains properties used to define a pawn.
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Ultra Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class ULTRAGAME_API UUltraPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UUltraPawnData(const FObjectInitializer& ObjectInitializer);

public:

	// Class to instantiate for this pawn (should usually derive from AUltraPawn or AUltraCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultra|Pawn")
	TSubclassOf<APawn> PawnClass;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultra|Abilities")
	TArray<TObjectPtr<UUltraAbilitySet>> AbilitySets;

	// What mapping of ability tags to use for actions taking by this pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultra|Abilities")
	TObjectPtr<UUltraAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultra|Input")
	TObjectPtr<UUltraInputConfig> InputConfig;

	// Default camera mode used by player controlled pawns.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultra|Camera")
	TSubclassOf<UUltraCameraMode> DefaultCameraMode;
};
