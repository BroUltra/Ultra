// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraTeamPublicInfo.h"

#include "Net/UnrealNetwork.h"
#include "Teams/UltraTeamInfoBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraTeamPublicInfo)

class FLifetimeProperty;

AUltraTeamPublicInfo::AUltraTeamPublicInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AUltraTeamPublicInfo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, TeamDisplayAsset, COND_InitialOnly);
}

void AUltraTeamPublicInfo::SetTeamDisplayAsset(TObjectPtr<UUltraTeamDisplayAsset> NewDisplayAsset)
{
	check(HasAuthority());
	check(TeamDisplayAsset == nullptr);

	TeamDisplayAsset = NewDisplayAsset;

	TryRegisterWithTeamSubsystem();
}

void AUltraTeamPublicInfo::OnRep_TeamDisplayAsset()
{
	TryRegisterWithTeamSubsystem();
}

