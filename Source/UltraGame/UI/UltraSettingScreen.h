// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataTable.h"
#include "Input/UIActionBindingHandle.h"
#include "UObject/UObjectGlobals.h"
#include "Widgets/GameSettingScreen.h"

#include "UltraSettingScreen.generated.h"

class UGameSettingRegistry;
class UUltraTabListWidgetBase;
class UObject;

UCLASS(Abstract, meta = (Category = "Settings", DisableNativeTick))
class UUltraSettingScreen : public UGameSettingScreen
{
	GENERATED_BODY()

public:

protected:
	virtual void NativeOnInitialized() override;
	virtual UGameSettingRegistry* CreateRegistry() override;

	void HandleBackAction();
	void HandleApplyAction();
	void HandleCancelChangesAction();

	virtual void OnSettingsDirtyStateChanged_Implementation(bool bSettingsDirty) override;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = Input, meta = (BindWidget, OptionalWidget = true, AllowPrivateAccess = true))
	TObjectPtr<UUltraTabListWidgetBase> TopSettingsTabs;
	
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle BackInputActionData;

	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle ApplyInputActionData;

	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle CancelChangesInputActionData;

	FUIActionBindingHandle BackHandle;
	FUIActionBindingHandle ApplyHandle;
	FUIActionBindingHandle CancelChangesHandle;
};
