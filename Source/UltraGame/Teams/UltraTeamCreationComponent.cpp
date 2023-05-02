// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraTeamCreationComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameModes/UltraExperienceDefinition.h"
#include "GameModes/UltraExperienceManagerComponent.h"
#include "UltraTeamPublicInfo.h"
#include "UltraTeamPrivateInfo.h"
#include "GameFramework/PlayerState.h"
#include "Player/UltraPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/World.h"
#include "GameModes/UltraGameMode.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraTeamCreationComponent)

UUltraTeamCreationComponent::UUltraTeamCreationComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PublicTeamInfoClass = AUltraTeamPublicInfo::StaticClass();
	PrivateTeamInfoClass = AUltraTeamPrivateInfo::StaticClass();
}

#if WITH_EDITOR
EDataValidationResult UUltraTeamCreationComponent::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = Super::IsDataValid(ValidationErrors);

	//@TODO: TEAMS: Validate that all display assets have the same properties set!

	return Result;
}
#endif

void UUltraTeamCreationComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for the experience load to complete
	AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
	UUltraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UUltraExperienceManagerComponent>();
	check(ExperienceComponent);
	ExperienceComponent->CallOrRegister_OnExperienceLoaded_HighPriority(FOnUltraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void UUltraTeamCreationComponent::OnExperienceLoaded(const UUltraExperienceDefinition* Experience)
{
#if WITH_SERVER_CODE
	if (HasAuthority())
	{
		ServerCreateTeams();
		ServerAssignPlayersToTeams();
	}
#endif
}

#if WITH_SERVER_CODE

void UUltraTeamCreationComponent::ServerCreateTeams()
{
	for (const auto& KVP : TeamsToCreate)
	{
		const int32 TeamId = KVP.Key;
		ServerCreateTeam(TeamId, KVP.Value);
	}
}

void UUltraTeamCreationComponent::ServerAssignPlayersToTeams()
{
	// Assign players that already exist to teams
	AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (AUltraPlayerState* UltraPS = Cast<AUltraPlayerState>(PS))
		{
			ServerChooseTeamForPlayer(UltraPS);
		}
	}

	// Listen for new players logging in
	AUltraGameMode* GameMode = Cast<AUltraGameMode>(GameState->AuthorityGameMode);
	check(GameMode);

	GameMode->OnGameModeCombinedPostLogin().AddUObject(this, &ThisClass::OnPostLogin);
}

void UUltraTeamCreationComponent::ServerChooseTeamForPlayer(AUltraPlayerState* PS)
{
	if (PS->IsOnlyASpectator())
	{
		PS->SetGenericTeamId(FGenericTeamId::NoTeam);
	}
	else
	{
		const FGenericTeamId TeamID = IntegerToGenericTeamId(GetLeastPopulatedTeamID());
		PS->SetGenericTeamId(TeamID);
	}
}

void UUltraTeamCreationComponent::OnPostLogin(AGameModeBase* GameMode, AController* NewPlayer)
{
	check(NewPlayer);
	check(NewPlayer->PlayerState);
	if (AUltraPlayerState* UltraPS = Cast<AUltraPlayerState>(NewPlayer->PlayerState))
	{
		ServerChooseTeamForPlayer(UltraPS);
	}
}

void UUltraTeamCreationComponent::ServerCreateTeam(int32 TeamId, UUltraTeamDisplayAsset* DisplayAsset)
{
	check(HasAuthority());

	//@TODO: ensure the team doesn't already exist

	UWorld* World = GetWorld();
	check(World);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AUltraTeamPublicInfo* NewTeamPublicInfo = World->SpawnActor<AUltraTeamPublicInfo>(PublicTeamInfoClass, SpawnInfo);
	checkf(NewTeamPublicInfo != nullptr, TEXT("Failed to create public team actor from class %s"), *GetPathNameSafe(*PublicTeamInfoClass));
	NewTeamPublicInfo->SetTeamId(TeamId);
	NewTeamPublicInfo->SetTeamDisplayAsset(DisplayAsset);

	AUltraTeamPrivateInfo* NewTeamPrivateInfo = World->SpawnActor<AUltraTeamPrivateInfo>(PrivateTeamInfoClass, SpawnInfo);
	checkf(NewTeamPrivateInfo != nullptr, TEXT("Failed to create private team actor from class %s"), *GetPathNameSafe(*PrivateTeamInfoClass));
	NewTeamPrivateInfo->SetTeamId(TeamId);
}

int32 UUltraTeamCreationComponent::GetLeastPopulatedTeamID() const
{
	const int32 NumTeams = TeamsToCreate.Num();
	if (NumTeams > 0)
	{
		TMap<int32, uint32> TeamMemberCounts;
		TeamMemberCounts.Reserve(NumTeams);

		for (const auto& KVP : TeamsToCreate)
		{
			const int32 TeamId = KVP.Key;
			TeamMemberCounts.Add(TeamId, 0);
		}

		AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
		for (APlayerState* PS : GameState->PlayerArray)
		{
			if (AUltraPlayerState* UltraPS = Cast<AUltraPlayerState>(PS))
			{
				const int32 PlayerTeamID = UltraPS->GetTeamId();

				if ((PlayerTeamID != INDEX_NONE) && !UltraPS->IsInactive())	// do not count unassigned or disconnected players
				{
					check(TeamMemberCounts.Contains(PlayerTeamID))
					TeamMemberCounts[PlayerTeamID] += 1;
				}
			}
		}

		// sort by lowest team population, then by team ID
		int32 BestTeamId = INDEX_NONE;
		uint32 BestPlayerCount = TNumericLimits<uint32>::Max();
		for (const auto& KVP : TeamMemberCounts)
		{
			const int32 TestTeamId = KVP.Key;
			const uint32 TestTeamPlayerCount = KVP.Value;

			if (TestTeamPlayerCount < BestPlayerCount)
			{
				BestTeamId = TestTeamId;
				BestPlayerCount = TestTeamPlayerCount;
			}
			else if (TestTeamPlayerCount == BestPlayerCount)
			{
				if ((TestTeamId < BestTeamId) || (BestTeamId == INDEX_NONE))
				{
					BestTeamId = TestTeamId;
					BestPlayerCount = TestTeamPlayerCount;
				}
			}
		}

		return BestTeamId;
	}

	return INDEX_NONE;
}
#endif	// WITH_SERVER_CODE

