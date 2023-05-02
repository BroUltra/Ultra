// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonInputBaseTypes.h"
#include "Input/CommonBoundActionButton.h"
#include "Templates/SubclassOf.h"
#include "UObject/UObjectGlobals.h"

#include "UltraBoundActionButton.generated.h"

class UCommonButtonStyle;
class UObject;

/**
 * 
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class ULTRAGAME_API UUltraBoundActionButton : public UCommonBoundActionButton
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	void HandleInputMethodChanged(ECommonInputType NewInputMethod);

	UPROPERTY(EditAnywhere, Category = "Styles")
	TSubclassOf<UCommonButtonStyle> KeyboardStyle;

	UPROPERTY(EditAnywhere, Category = "Styles")
	TSubclassOf<UCommonButtonStyle> GamepadStyle;

	UPROPERTY(EditAnywhere, Category = "Styles")
	TSubclassOf<UCommonButtonStyle> TouchStyle;
};
