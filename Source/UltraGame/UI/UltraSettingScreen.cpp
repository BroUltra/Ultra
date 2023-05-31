// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraSettingScreen.h"

#include "Input/CommonUIInputTypes.h"
#include "Player/UltraLocalPlayer.h"
#include "Settings/UltraGameSettingRegistry.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraSettingScreen)

class UGameSettingRegistry;

void UUltraSettingScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleBackAction)));
	ApplyHandle = RegisterUIActionBinding(FBindUIActionArgs(ApplyInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleApplyAction)));
	CancelChangesHandle = RegisterUIActionBinding(FBindUIActionArgs(CancelChangesInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleCancelChangesAction)));
}

UGameSettingRegistry* UUltraSettingScreen::CreateRegistry()
{
	UUltraGameSettingRegistry* NewRegistry = NewObject<UUltraGameSettingRegistry>();

	if (UUltraLocalPlayer* LocalPlayer = CastChecked<UUltraLocalPlayer>(GetOwningLocalPlayer()))
	{
		NewRegistry->Initialize(LocalPlayer);
	}

	return NewRegistry;
}

void UUltraSettingScreen::HandleBackAction()
{
	if (AttemptToPopNavigation())
	{
		return;
	}

	ApplyChanges();

	DeactivateWidget();
}

void UUltraSettingScreen::HandleApplyAction()
{
	ApplyChanges();
}

void UUltraSettingScreen::HandleCancelChangesAction()
{
	CancelChanges();
}

void UUltraSettingScreen::OnSettingsDirtyStateChanged_Implementation(bool bSettingsDirty)
{
	if (bSettingsDirty)
	{
		if (!GetActionBindings().Contains(ApplyHandle))
		{
			AddActionBinding(ApplyHandle);
		}
		if (!GetActionBindings().Contains(CancelChangesHandle))
		{
			AddActionBinding(CancelChangesHandle);
		}
	}
	else
	{
		RemoveActionBinding(ApplyHandle);
		RemoveActionBinding(CancelChangesHandle);
	}
}
