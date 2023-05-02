// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraCosmeticCheats.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "UltraControllerComponent_CharacterParts.h"
#include "System/UltraDevelopmentStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraCosmeticCheats)

//////////////////////////////////////////////////////////////////////
// UUltraCosmeticCheats

UUltraCosmeticCheats::UUltraCosmeticCheats()
{
#if UE_WITH_CHEAT_MANAGER
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

void UUltraCosmeticCheats::AddCharacterPart(const FString& AssetName, bool bSuppressNaturalParts)
{
#if UE_WITH_CHEAT_MANAGER
	if (UUltraControllerComponent_CharacterParts* CosmeticComponent = GetCosmeticComponent())
	{
		TSubclassOf<AActor> PartClass = UUltraDevelopmentStatics::FindClassByShortName<AActor>(AssetName);
		if (PartClass != nullptr)
		{
			FUltraCharacterPart Part;
			Part.PartClass = PartClass;

			CosmeticComponent->AddCheatPart(Part, bSuppressNaturalParts);
		}
	}
#endif	
}

void UUltraCosmeticCheats::ReplaceCharacterPart(const FString& AssetName, bool bSuppressNaturalParts)
{
	ClearCharacterPartOverrides();
	AddCharacterPart(AssetName, bSuppressNaturalParts);
}

void UUltraCosmeticCheats::ClearCharacterPartOverrides()
{
#if UE_WITH_CHEAT_MANAGER
	if (UUltraControllerComponent_CharacterParts* CosmeticComponent = GetCosmeticComponent())
	{
		CosmeticComponent->ClearCheatParts();
	}
#endif	
}

UUltraControllerComponent_CharacterParts* UUltraCosmeticCheats::GetCosmeticComponent() const
{
	if (APlayerController* PC = GetPlayerController())
	{
		return PC->FindComponentByClass<UUltraControllerComponent_CharacterParts>();
	}

	return nullptr;
}

