// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonUserWidget.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "UltraWeaponUserInterface.generated.h"

class UUltraWeaponInstance;
class UObject;
struct FGeometry;

UCLASS()
class UUltraWeaponUserInterface : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UUltraWeaponUserInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponChanged(UUltraWeaponInstance* OldWeapon, UUltraWeaponInstance* NewWeapon);

private:
	void RebuildWidgetFromWeapon();

private:
	UPROPERTY(Transient)
	TObjectPtr<UUltraWeaponInstance> CurrentInstance;
};
