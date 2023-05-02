// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UltraTeamInfoBase.h"
#include "UObject/UObjectGlobals.h"

#include "UltraTeamPrivateInfo.generated.h"

class UObject;

UCLASS()
class AUltraTeamPrivateInfo : public AUltraTeamInfoBase
{
	GENERATED_BODY()

public:
	AUltraTeamPrivateInfo(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
