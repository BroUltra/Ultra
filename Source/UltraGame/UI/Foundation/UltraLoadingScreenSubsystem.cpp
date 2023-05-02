// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/Foundation/UltraLoadingScreenSubsystem.h"

#include "Blueprint/UserWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraLoadingScreenSubsystem)

class UUserWidget;

//////////////////////////////////////////////////////////////////////
// UUltraLoadingScreenSubsystem

UUltraLoadingScreenSubsystem::UUltraLoadingScreenSubsystem()
{
}

void UUltraLoadingScreenSubsystem::SetLoadingScreenContentWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (LoadingScreenWidgetClass != NewWidgetClass)
	{
		LoadingScreenWidgetClass = NewWidgetClass;

		OnLoadingScreenWidgetChanged.Broadcast(LoadingScreenWidgetClass);
	}
}

TSubclassOf<UUserWidget> UUltraLoadingScreenSubsystem::GetLoadingScreenContentWidget() const
{
	return LoadingScreenWidgetClass;
}

