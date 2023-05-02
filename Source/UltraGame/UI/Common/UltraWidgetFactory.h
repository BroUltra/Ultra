// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Templates/SubclassOf.h"
#include "UObject/Object.h"
#include "UObject/UObjectGlobals.h"

#include "UltraWidgetFactory.generated.h"

class UUserWidget;
struct FFrame;

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class ULTRAGAME_API UUltraWidgetFactory : public UObject
{
	GENERATED_BODY()

public:
	UUltraWidgetFactory() { }

	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<UUserWidget> FindWidgetClassForData(const UObject* Data) const;
};