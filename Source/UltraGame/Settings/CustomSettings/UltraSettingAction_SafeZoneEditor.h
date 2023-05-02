// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/Array.h"
#include "GameSettingAction.h"
#include "GameSettingValueScalarDynamic.h"
#include "UObject/UObjectGlobals.h"

#include "UltraSettingAction_SafeZoneEditor.generated.h"

class UGameSetting;
class UObject;


UCLASS()
class UUltraSettingValueScalarDynamic_SafeZoneValue : public UGameSettingValueScalarDynamic
{
	GENERATED_BODY()

public:
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;
};

UCLASS()
class UUltraSettingAction_SafeZoneEditor : public UGameSettingAction
{
	GENERATED_BODY()
	
public:
	UUltraSettingAction_SafeZoneEditor();
	virtual TArray<UGameSetting*> GetChildSettings() override;

private:
	UPROPERTY()
	TObjectPtr<UUltraSettingValueScalarDynamic_SafeZoneValue> SafeZoneValueSetting;
};