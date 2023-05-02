// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraTabButtonBase.h"

#include "CommonLazyImage.h"
#include "UI/Common/UltraTabListWidgetBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraTabButtonBase)

class UObject;
struct FSlateBrush;

void UUltraTabButtonBase::SetIconFromLazyObject(TSoftObjectPtr<UObject> LazyObject)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrushFromLazyDisplayAsset(LazyObject);
	}
}

void UUltraTabButtonBase::SetIconBrush(const FSlateBrush& Brush)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrush(Brush);
	}
}

void UUltraTabButtonBase::SetTabLabelInfo_Implementation(const FUltraTabDescriptor& TabLabelInfo)
{
	SetButtonText(TabLabelInfo.TabText);
	SetIconBrush(TabLabelInfo.IconBrush);
}

