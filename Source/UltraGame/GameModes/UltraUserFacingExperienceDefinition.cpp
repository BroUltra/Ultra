// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraUserFacingExperienceDefinition.h"

#include "CommonSessionSubsystem.h"
#include "CommonUISettings.h"
#include "Containers/UnrealString.h"
#include "ICommonUIModule.h"
#include "NativeGameplayTags.h"
#include "UObject/NameTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraUserFacingExperienceDefinition)

namespace Ultra::Experience
{
	UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Platform_Trait_ReplaySupport, "Platform.Trait.ReplaySupport");
};

UCommonSession_HostSessionRequest* UUltraUserFacingExperienceDefinition::CreateHostingRequest() const
{
	const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();
	const FString UserFacingExperienceName = GetPrimaryAssetId().PrimaryAssetName.ToString();
	UCommonSession_HostSessionRequest* Result = NewObject<UCommonSession_HostSessionRequest>();
	Result->OnlineMode = ECommonSessionOnlineMode::Online;
	Result->bUseLobbies = true;
	Result->MapID = MapID;
	Result->ModeNameForAdvertisement = UserFacingExperienceName;
	Result->ExtraArgs = ExtraArgs;
	Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);
	Result->MaxPlayerCount = MaxPlayerCount;

	if (ICommonUIModule::GetSettings().GetPlatformTraits().HasTag(Ultra::Experience::TAG_Platform_Trait_ReplaySupport.GetTag()))
	{
		if (bRecordReplay)
		{
			Result->ExtraArgs.Add(TEXT("DemoRec"), FString());
		}
	}

	return Result;
}
