// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonUserWidget.h"
#include "Performance/UltraPerformanceStatTypes.h"

#include "UltraPerfStatContainerBase.generated.h"

class UObject;
struct FFrame;

/**
 * UUltraPerfStatsContainerBase
 *
 * Panel that contains a set of UUltraPerfStatWidgetBase widgets and manages
 * their visibility based on user settings.
 */
 UCLASS(Abstract)
class UUltraPerfStatContainerBase : public UCommonUserWidget
{
public:
	UUltraPerfStatContainerBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	GENERATED_BODY()

	//~UUserWidget interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//~End of UUserWidget interface

	UFUNCTION(BlueprintCallable)
	void UpdateVisibilityOfChildren();

protected:
	// Are we showing text or graph stats?
	UPROPERTY(EditAnywhere, Category=Display)
	EUltraStatDisplayMode StatDisplayModeFilter = EUltraStatDisplayMode::TextAndGraph;
};
