// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonListView.h"
#include "Components/ListView.h"

#include "UltraListView.generated.h"

class UUserWidget;
class ULocalPlayer;
class UUltraWidgetFactory;

UCLASS(meta = (DisableNativeTick))
class ULTRAGAME_API UUltraListView : public UCommonListView
{
	GENERATED_BODY()

public:
	UUltraListView(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& InCompileLog) const override;
#endif

protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	//virtual bool OnIsSelectableOrNavigableInternal(UObject* SelectedItem) override;

protected:
	UPROPERTY(EditAnywhere, Instanced, Category="Entry Creation")
	TArray<TObjectPtr<UUltraWidgetFactory>> FactoryRules;
};