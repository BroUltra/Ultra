// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "UltraHitPopStyleNiagara.generated.h"

class UNiagaraSystem;

/*PopStyle is used to define what Niagara asset should be used for the Hit System representation*/
UCLASS()
class UUltraHitPopStyleNiagara : public UDataAsset
{
	GENERATED_BODY()

public:

	//Name of the Niagara Array to set the Hit information
	UPROPERTY(EditDefaultsOnly, Category="HitPop")
	FName NiagaraArrayName;

	//Niagara System used to display the hits
	UPROPERTY(EditDefaultsOnly, Category="HitPop")
	TObjectPtr<UNiagaraSystem> TextNiagara;
};
