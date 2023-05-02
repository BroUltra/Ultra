// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/EngineTypes.h"
#include "GameFramework/Info.h"
#include "HAL/Platform.h"
#include "System/GameplayTagStack.h"
#include "UObject/UObjectGlobals.h"

#include "UltraTeamInfoBase.generated.h"

class UUltraTeamCreationComponent;
class UUltraTeamSubsystem;
class UObject;
struct FFrame;

UCLASS(Abstract)
class AUltraTeamInfoBase : public AInfo
{
	GENERATED_BODY()

public:
	AUltraTeamInfoBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	int32 GetTeamId() const { return TeamId; }

	//~AActor interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface

protected:
	virtual void RegisterWithTeamSubsystem(UUltraTeamSubsystem* Subsystem);
	void TryRegisterWithTeamSubsystem();

private:
	void SetTeamId(int32 NewTeamId);

	UFUNCTION()
	void OnRep_TeamId();

public:
	friend UUltraTeamCreationComponent;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer TeamTags;

private:
	UPROPERTY(ReplicatedUsing=OnRep_TeamId)
	int32 TeamId;
};
