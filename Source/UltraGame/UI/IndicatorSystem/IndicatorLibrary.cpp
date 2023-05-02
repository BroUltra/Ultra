// Copyright Epic Games, Inc. All Rights Reserved.

#include "IndicatorLibrary.h"

#include "UltraIndicatorManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IndicatorLibrary)

class AController;

UIndicatorLibrary::UIndicatorLibrary()
{
}

UUltraIndicatorManagerComponent* UIndicatorLibrary::GetIndicatorManagerComponent(AController* Controller)
{
	return UUltraIndicatorManagerComponent::GetComponent(Controller);
}

