// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/Map.h"
#include "UltraWidgetFactory.h"
#include "Templates/SubclassOf.h"
#include "UObject/SoftObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "UltraWidgetFactory_Class.generated.h"

class UObject;
class UUserWidget;

UCLASS()
class ULTRAGAME_API UUltraWidgetFactory_Class : public UUltraWidgetFactory
{
	GENERATED_BODY()

public:
	UUltraWidgetFactory_Class() { }

	virtual TSubclassOf<UUserWidget> FindWidgetClassForData_Implementation(const UObject* Data) const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = ListEntries, meta = (AllowAbstract))
	TMap<TSoftClassPtr<UObject>, TSubclassOf<UUserWidget>> EntryWidgetForClass;
};