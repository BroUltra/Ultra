// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraBotCheats.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "GameModes/UltraBotCreationComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraBotCheats)

//////////////////////////////////////////////////////////////////////
// UUltraBotCheats

UUltraBotCheats::UUltraBotCheats()
{
#if WITH_SERVER_CODE && UE_WITH_CHEAT_MANAGER
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		UCheatManager::RegisterForOnCheatManagerCreated(FOnCheatManagerCreated::FDelegate::CreateLambda(
			[](UCheatManager* CheatManager)
			{
				CheatManager->AddCheatManagerExtension(NewObject<ThisClass>(CheatManager));
			}));
	}
#endif
}

void UUltraBotCheats::AddPlayerBot()
{
#if WITH_SERVER_CODE && UE_WITH_CHEAT_MANAGER
	if (UUltraBotCreationComponent* BotComponent = GetBotComponent())
	{
		BotComponent->Cheat_AddBot();
	}
#endif	
}

void UUltraBotCheats::RemovePlayerBot()
{
#if WITH_SERVER_CODE && UE_WITH_CHEAT_MANAGER
	if (UUltraBotCreationComponent* BotComponent = GetBotComponent())
	{
		BotComponent->Cheat_RemoveBot();
	}
#endif	
}

UUltraBotCreationComponent* UUltraBotCheats::GetBotComponent() const
{
	if (UWorld* World = GetWorld())
	{
		if (AGameStateBase* GameState = World->GetGameState())
		{
			return GameState->FindComponentByClass<UUltraBotCreationComponent>();
		}
	}

	return nullptr;
}

