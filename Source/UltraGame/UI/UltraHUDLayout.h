// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UltraActivatableWidget.h"

#include "UltraHUDLayout.generated.h"

class UCommonActivatableWidget;
class UObject;


/**
 * UUltraHUDLayout
 *
 *	Widget used to lay out the player's HUD (typically specified by an Add Widgets action in the experience)
 */
UCLASS(Abstract, BlueprintType, Blueprintable, Meta = (DisplayName = "Ultra HUD Layout", Category = "Ultra|HUD"))
class UUltraHUDLayout : public UUltraActivatableWidget
{
	GENERATED_BODY()

public:

	UUltraHUDLayout(const FObjectInitializer& ObjectInitializer);

	void NativeOnInitialized() override;

protected:
	void HandleEscapeAction();

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> EscapeMenuClass;
};
