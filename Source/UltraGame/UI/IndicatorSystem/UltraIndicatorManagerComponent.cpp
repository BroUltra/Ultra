// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraIndicatorManagerComponent.h"

#include "GameFramework/Controller.h"
#include "IndicatorDescriptor.h"
#include "Misc/AssertionMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraIndicatorManagerComponent)

UUltraIndicatorManagerComponent::UUltraIndicatorManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoRegister = true;
	bAutoActivate = true;
}

/*static*/ UUltraIndicatorManagerComponent* UUltraIndicatorManagerComponent::GetComponent(AController* Controller)
{
	if (Controller)
	{
		return Controller->FindComponentByClass<UUltraIndicatorManagerComponent>();
	}

	return nullptr;
}

void UUltraIndicatorManagerComponent::AddIndicator(UIndicatorDescriptor* IndicatorDescriptor)
{
	IndicatorDescriptor->SetIndicatorManagerComponent(this);
	OnIndicatorAdded.Broadcast(IndicatorDescriptor);
	Indicators.Add(IndicatorDescriptor);
}

void UUltraIndicatorManagerComponent::RemoveIndicator(UIndicatorDescriptor* IndicatorDescriptor)
{
	if (IndicatorDescriptor)
	{
		ensure(IndicatorDescriptor->GetIndicatorManagerComponent() == this);
	
		OnIndicatorRemoved.Broadcast(IndicatorDescriptor);
		Indicators.Remove(IndicatorDescriptor);
	}
}
