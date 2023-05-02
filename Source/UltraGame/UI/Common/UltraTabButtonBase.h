// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UltraTabListWidgetBase.h"
#include "UI/Foundation/UltraButtonBase.h"
#include "UObject/SoftObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "UltraTabButtonBase.generated.h"

class UCommonLazyImage;
class UObject;
struct FFrame;
struct FSlateBrush;

UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick))
class ULTRAGAME_API UUltraTabButtonBase : public UUltraButtonBase, public IUltraTabButtonInterface
{
	GENERATED_BODY()

public:

	void SetIconFromLazyObject(TSoftObjectPtr<UObject> LazyObject);
	void SetIconBrush(const FSlateBrush& Brush);

protected:

	UFUNCTION()
	virtual void SetTabLabelInfo_Implementation(const FUltraTabDescriptor& TabLabelInfo) override;

private:

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonLazyImage> LazyImage_Icon;
};
