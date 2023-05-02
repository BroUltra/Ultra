// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/UObjectGlobals.h"

#include "IndicatorLibrary.generated.h"

class AController;
class UUltraIndicatorManagerComponent;
class UObject;
struct FFrame;

UCLASS()
class ULTRAGAME_API UIndicatorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UIndicatorLibrary();
	
	/**  */
	UFUNCTION(BlueprintCallable, Category = Indicator)
	static UUltraIndicatorManagerComponent* GetIndicatorManagerComponent(AController* Controller);
};
