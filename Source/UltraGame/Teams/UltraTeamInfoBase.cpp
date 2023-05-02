// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraTeamInfoBase.h"

#include "Containers/Array.h"
#include "CoreTypes.h"
#include "Engine/World.h"
#include "Misc/AssertionMacros.h"
#include "Net/UnrealNetwork.h"
#include "Teams/UltraTeamSubsystem.h"
#include "UObject/CoreNetTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraTeamInfoBase)

class FLifetimeProperty;

AUltraTeamInfoBase::AUltraTeamInfoBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, TeamId(INDEX_NONE)
{
	bReplicates = true;
	bAlwaysRelevant = true;
	NetPriority = 3.0f;
	SetReplicatingMovement(false);
}

void AUltraTeamInfoBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TeamTags);
	DOREPLIFETIME_CONDITION(ThisClass, TeamId, COND_InitialOnly);
}

void AUltraTeamInfoBase::BeginPlay()
{
	Super::BeginPlay();

	TryRegisterWithTeamSubsystem();
}

void AUltraTeamInfoBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (TeamId != INDEX_NONE)
	{
		UUltraTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UUltraTeamSubsystem>();
		TeamSubsystem->UnregisterTeamInfo(this);
	}

	Super::EndPlay(EndPlayReason);
}

void AUltraTeamInfoBase::RegisterWithTeamSubsystem(UUltraTeamSubsystem* Subsystem)
{
	Subsystem->RegisterTeamInfo(this);
}

void AUltraTeamInfoBase::TryRegisterWithTeamSubsystem()
{
	if (TeamId != INDEX_NONE)
	{
		UUltraTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UUltraTeamSubsystem>();
		RegisterWithTeamSubsystem(TeamSubsystem);
	}
}

void AUltraTeamInfoBase::SetTeamId(int32 NewTeamId)
{
	check(HasAuthority());
	check(TeamId == INDEX_NONE);
	check(NewTeamId != INDEX_NONE);

	TeamId = NewTeamId;

	TryRegisterWithTeamSubsystem();
}

void AUltraTeamInfoBase::OnRep_TeamId()
{
	TryRegisterWithTeamSubsystem();
}

