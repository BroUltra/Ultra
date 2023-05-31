// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraWidgetFactory.h"
#include "Templates/SubclassOf.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraWidgetFactory)

class UUserWidget;

TSubclassOf<UUserWidget> UUltraWidgetFactory::FindWidgetClassForData_Implementation(const UObject* Data) const
{
	return TSubclassOf<UUserWidget>();
}
