// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UltraNumberPopComponent.h"

#include "UltraNumberPopComponent_NiagaraText.generated.h"

class UUltraHitPopStyleNiagara;
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
	
	TArray<int32> HitNumberArray;

	/** Style patterns to attempt to apply to the incoming number pops */
	UPROPERTY(EditDefaultsOnly, Category = "Number Pop|Style")
	TObjectPtr<UUltraHitPopStyleNiagara> Style;

	//Niagara Component used to display the hit
	UPROPERTY(EditDefaultsOnly, Category = "Number Pop|Style")
	TObjectPtr<UNiagaraComponent> NiagaraComp;
};
