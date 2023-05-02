// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UltraTeamInfoBase.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "UltraTeamPublicInfo.generated.h"

class UUltraTeamCreationComponent;
class UUltraTeamDisplayAsset;
class UObject;
struct FFrame;

UCLASS()
class AUltraTeamPublicInfo : public AUltraTeamInfoBase
{
	GENERATED_BODY()

	friend UUltraTeamCreationComponent;

public:
	AUltraTeamPublicInfo(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UUltraTeamDisplayAsset* GetTeamDisplayAsset() const { return TeamDisplayAsset; }

private:
	UFUNCTION()
	void OnRep_TeamDisplayAsset();

	void SetTeamDisplayAsset(TObjectPtr<UUltraTeamDisplayAsset> NewDisplayAsset);

private:
	UPROPERTY(ReplicatedUsing=OnRep_TeamDisplayAsset)
	TObjectPtr<UUltraTeamDisplayAsset> TeamDisplayAsset;
};
