// Copyright Epic Games, Inc. All Rights Reserved.

#include "Battle_PlayerSpawningManagementComponent.h"

#include "CoreTypes.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerState.h"
#include "GameModes/UltraGameState.h"
#include "Misc/AssertionMacros.h"
#include "Player/UltraPlayerStart.h"
#include "Teams/UltraTeamSubsystem.h"
#include "UObject/ObjectPtr.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(Battle_PlayerSpawningManagementComponent)

class AActor;

UBattle_PlayerSpawningManagementComponent::UBattle_PlayerSpawningManagementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

AActor* UBattle_PlayerSpawningManagementComponent::OnChoosePlayerStart(AController* Player, TArray<AUltraPlayerStart*>& PlayerStarts)
{
	UUltraTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UUltraTeamSubsystem>();
	const int32 PlayerTeamId = TeamSubsystem->FindTeamFromObject(Player);

	// We should have a TeamId by now, but early login stuff before post login can try to do stuff, ignore it.
	if (!ensure(PlayerTeamId != INDEX_NONE))
	{
		return nullptr;
	}

	AUltraGameState* GameState = GetGameStateChecked<AUltraGameState>();

	AUltraPlayerStart* BestPlayerStart = nullptr;
	double MaxDistance = 0;
	AUltraPlayerStart* FallbackPlayerStart = nullptr;
	double FallbackMaxDistance = 0;

	for (APlayerState* PS : GameState->PlayerArray)
	{
		const int32 TeamId = TeamSubsystem->FindTeamFromObject(PS);
		
		// We should have a TeamId by now...
		if (PS->IsOnlyASpectator() || !ensure(TeamId != INDEX_NONE))
		{
			continue;
		}

		// If the other player isn't on the same team, lets find the furthest spawn from them.
		if (TeamId != PlayerTeamId)
		{
			for (AUltraPlayerStart* PlayerStart : PlayerStarts)
			{
				if (APawn* Pawn = PS->GetPawn())
				{
					const double Distance = PlayerStart->GetDistanceTo(Pawn);

					if (PlayerStart->IsClaimed())
					{
						if (FallbackPlayerStart == nullptr || Distance > FallbackMaxDistance)
						{
							FallbackPlayerStart = PlayerStart;
							FallbackMaxDistance = Distance;
						}
					}
					else if (PlayerStart->GetLocationOccupancy(Player) < EUltraPlayerStartLocationOccupancy::Full)
					{
						if (BestPlayerStart == nullptr || Distance > MaxDistance)
						{
							BestPlayerStart = PlayerStart;
							MaxDistance = Distance;
						}
					}
				}
			}
		}
	}

	if (BestPlayerStart)
	{
		return BestPlayerStart;
	}

	return FallbackPlayerStart;
}

void UBattle_PlayerSpawningManagementComponent::OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation)
{
	
}
