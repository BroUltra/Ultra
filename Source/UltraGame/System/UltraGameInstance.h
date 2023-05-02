// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonGameInstance.h"
#include "UObject/UObjectGlobals.h"

#include "UltraGameInstance.generated.h"

class AUltraPlayerController;
class UObject;

UCLASS(Config = Game)
class ULTRAGAME_API UUltraGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()

public:

	UUltraGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	AUltraPlayerController* GetPrimaryPlayerController() const;
	
	virtual bool CanJoinRequestedSession() const override;

protected:

	virtual void Init() override;
	virtual void Shutdown() override;
};
