// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayEffectTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UltraVerbMessage.h"
#include "UObject/UObjectGlobals.h"

#include "UltraVerbMessageHelpers.generated.h"

class APlayerController;
class APlayerState;
class UObject;
struct FFrame;


UCLASS()
class ULTRAGAME_API UUltraVerbMessageHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ultra")
	static APlayerState* GetPlayerStateFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Ultra")
	static APlayerController* GetPlayerControllerFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Ultra")
	static FGameplayCueParameters VerbMessageToCueParameters(const FUltraVerbMessage& Message);

	UFUNCTION(BlueprintCallable, Category = "Ultra")
	static FUltraVerbMessage CueParametersToVerbMessage(const FGameplayCueParameters& Params);
};
