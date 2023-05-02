// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraPerfStatContainerBase.h"

#include "Blueprint/WidgetTree.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/Widget.h"
#include "Delegates/Delegate.h"
#include "UltraPerfStatWidgetBase.h"
#include "Misc/AssertionMacros.h"
#include "Settings/UltraSettingsLocal.h"
#include "Templates/Casts.h"
#include "UObject/ObjectPtr.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraPerfStatContainerBase)

//////////////////////////////////////////////////////////////////////
// UUltraPerfStatsContainerBase

UUltraPerfStatContainerBase::UUltraPerfStatContainerBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUltraPerfStatContainerBase::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateVisibilityOfChildren();

	UUltraSettingsLocal::Get()->OnPerfStatDisplayStateChanged().AddUObject(this, &ThisClass::UpdateVisibilityOfChildren);
}

void UUltraPerfStatContainerBase::NativeDestruct()
{
	UUltraSettingsLocal::Get()->OnPerfStatDisplayStateChanged().RemoveAll(this);

	Super::NativeDestruct();
}

void UUltraPerfStatContainerBase::UpdateVisibilityOfChildren()
{
	UUltraSettingsLocal* UserSettings = UUltraSettingsLocal::Get();

	const bool bShowTextWidgets = (StatDisplayModeFilter == EUltraStatDisplayMode::TextOnly) || (StatDisplayModeFilter == EUltraStatDisplayMode::TextAndGraph);
	const bool bShowGraphWidgets = (StatDisplayModeFilter == EUltraStatDisplayMode::GraphOnly) || (StatDisplayModeFilter == EUltraStatDisplayMode::TextAndGraph);
	
	check(WidgetTree);
	WidgetTree->ForEachWidget([&](UWidget* Widget)
	{
		if (UUltraPerfStatWidgetBase* TypedWidget = Cast<UUltraPerfStatWidgetBase>(Widget))
		{
			const EUltraStatDisplayMode SettingMode = UserSettings->GetPerfStatDisplayState(TypedWidget->GetStatToDisplay());

			bool bShowWidget = false;
			switch (SettingMode)
			{
			case EUltraStatDisplayMode::Hidden:
				bShowWidget = false;
				break;
			case EUltraStatDisplayMode::TextOnly:
				bShowWidget = bShowTextWidgets;
				break;
			case EUltraStatDisplayMode::GraphOnly:
				bShowWidget = bShowGraphWidgets;
				break;
			case EUltraStatDisplayMode::TextAndGraph:
				bShowWidget = bShowTextWidgets || bShowGraphWidgets;
				break;
			}

			TypedWidget->SetVisibility(bShowWidget ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		}
	});
}

