// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "UltraVerbMessageHelpers.generated.h"

struct FGameplayCueParameters;
struct FUltraVerbMessage;

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
