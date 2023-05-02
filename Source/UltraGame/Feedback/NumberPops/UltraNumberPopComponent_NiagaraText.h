// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/Array.h"
#include "HAL/Platform.h"
#include "UltraNumberPopComponent.h"
#include "UObject/UObjectGlobals.h"

#include "UltraNumberPopComponent_NiagaraText.generated.h"

class UUltraDamagePopStyleNiagara;
class UNiagaraComponent;
class UObject;

UCLASS(Blueprintable)
class UUltraNumberPopComponent_NiagaraText : public UUltraNumberPopComponent
{
	GENERATED_BODY()

public:

	UUltraNumberPopComponent_NiagaraText(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UUltraNumberPopComponent interface
	virtual void AddNumberPop(const FUltraNumberPopRequest& NewRequest) override;
	//~End of UUltraNumberPopComponent interface

protected:
	
	TArray<int32> DamageNumberArray;

	/** Style patterns to attempt to apply to the incoming number pops */
	UPROPERTY(EditDefaultsOnly, Category = "Number Pop|Style")
	TObjectPtr<UUltraDamagePopStyleNiagara> Style;

	//Niagara Component used to display the damage
	UPROPERTY(EditDefaultsOnly, Category = "Number Pop|Style")
	TObjectPtr<UNiagaraComponent> NiagaraComp;
};
