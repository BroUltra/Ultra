// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraHotfixManager.h"
#include "UObject/UObjectIterator.h"
#include "Engine/NetDriver.h"
#include "DeviceProfiles/DeviceProfileManager.h"
#include "DeviceProfiles/DeviceProfile.h"
#include "Settings/UltraSettingsLocal.h"
#include "HAL/MemoryMisc.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraHotfixManager)

int32 UUltraHotfixManager::GameHotfixCounter = 0;

UUltraHotfixManager::UUltraHotfixManager()
{
#if !UE_BUILD_SHIPPING
	OnScreenMessageHandle = FCoreDelegates::OnGetOnScreenMessages.AddUObject(this, &UUltraHotfixManager::GetOnScreenMessages);
#endif // !UE_BUILD_SHIPPING

	HotfixCompleteDelegateHandle = AddOnHotfixCompleteDelegate_Handle(FOnHotfixCompleteDelegate::CreateUObject(this, &ThisClass::OnHotfixCompleted));
}

void UUltraHotfixManager::Init()
{
	Super::Init();
}

void UUltraHotfixManager::OnHotfixCompleted(EHotfixResult HotfixResult)
{
	// Reload DDoS detection config for all live Net Drivers (mirrors RepGraph code)
	for (TObjectIterator<UNetDriver> It; It; ++It)
	{
		if (It->IsServer())
		{
			UE_LOG(LogHotfixManager, Log, TEXT("Reloading DDoS detection settings for NetDriver: %s"), *It->GetName());

			It->DDoS.InitConfig();
		}
	}

	if (bHasPendingDeviceProfileHotfix)
	{
		UE_LOG(LogHotfixManager, Log, TEXT("Re-applying Hotfixed DeviceProfile"));

		bHasPendingDeviceProfileHotfix = false;
		UDeviceProfileManager::Get().ReapplyDeviceProfile();

		UUltraSettingsLocal* GameSettings = UUltraSettingsLocal::Get();
		GameSettings->OnHotfixDeviceProfileApplied();
	}

#if ENABLE_SHARED_MEMORY_TRACKER
	FSharedMemoryTracker::PrintMemoryDiff(TEXT("Hotfix Complete"));
#endif
}

UUltraHotfixManager::~UUltraHotfixManager()
{
	ClearOnHotfixCompleteDelegate_Handle(HotfixCompleteDelegateHandle);

#if !UE_BUILD_SHIPPING
	FCoreDelegates::OnGetOnScreenMessages.Remove(OnScreenMessageHandle);
#endif // !UE_BUILD_SHIPPING
}

bool UUltraHotfixManager::WantsHotfixProcessing(const FCloudFileHeader& FileHeader)
{
	bool bWantsProcessing = Super::WantsHotfixProcessing(FileHeader);
	if (!bWantsProcessing)
	{
		FString SupportedFiles[] = {
			TEXT("AssetMigrations.ini")
		};

		for (FString SupportedFile : SupportedFiles)
		{
#if !UE_BUILD_SHIPPING
			if (!DebugPrefix.IsEmpty())
			{
				SupportedFile = DebugPrefix + SupportedFile;
			}
#endif
			if (SupportedFile == FileHeader.FileName)
			{
				bWantsProcessing = true;
				break;
			}
		}
	}
	return bWantsProcessing;
}

bool UUltraHotfixManager::HotfixIniFile(const FString& FileName, const FString& IniData)
{
	if (!bHasPendingDeviceProfileHotfix && FileName.EndsWith(TEXT("DEVICEPROFILES.INI"), ESearchCase::IgnoreCase))
	{
		FConfigFile DeviceProfileHotfixConfig;
		DeviceProfileHotfixConfig.CombineFromBuffer(IniData, FileName);
		TSet<FString> Keys;
		for (const auto& DPSection : DeviceProfileHotfixConfig)
		{
			FString DeviceProfileName, DeviceProfileClass;
			if (DPSection.Key.Split(TEXT(" "), &DeviceProfileName, &DeviceProfileClass) && DeviceProfileClass == *UDeviceProfile::StaticClass()->GetName())
			{
				Keys.Add(DeviceProfileName);
			}
		}

		// Check if any of the hotfixed device profiles are referenced by the currently active profile(s):
		bHasPendingDeviceProfileHotfix = UDeviceProfileManager::Get().DoActiveProfilesReference(Keys);
		UE_LOG(LogHotfixManager, Log, TEXT("Active device profile was referenced by hotfix = %d"), (uint32)bHasPendingDeviceProfileHotfix);
	}

	return Super::HotfixIniFile(FileName, IniData);
}

bool UUltraHotfixManager::ApplyHotfixProcessing(const FCloudFileHeader& FileHeader)
{
	// This allows json files to be downloaded automatically
	const FString Extension = FPaths::GetExtension(FileHeader.FileName);
	if (Extension == TEXT("json"))
	{
		return true;
	}
	
	const bool bResult = Super::ApplyHotfixProcessing(FileHeader);
	if (bResult && FileHeader.FileName.EndsWith(TEXT("GAME.INI"), ESearchCase::IgnoreCase))
	{
		GameHotfixCounter++;

		if (bHasPendingGameHotfix)
		{
			bHasPendingGameHotfix = false;
			OnPendingGameHotfixChanged.Broadcast(bHasPendingGameHotfix);
		}
	}

	return bResult;
}

bool UUltraHotfixManager::ShouldWarnAboutMissingWhenPatchingFromIni(const FString& AssetPath) const
{
	return AssetPath.StartsWith(TEXT("/Engine/")) || AssetPath.StartsWith(TEXT("/Game/"));
}

void UUltraHotfixManager::PatchAssetsFromIniFiles()
{
#if ENABLE_SHARED_MEMORY_TRACKER
	FSharedMemoryTracker::PrintMemoryDiff(TEXT("Start - PatchAssetsFromIniFiles"));
#endif
}

void UUltraHotfixManager::OnHotfixAvailablityCheck(const TArray<FCloudFileHeader>& PendingChangedFiles, const TArray<FCloudFileHeader>& PendingRemoveFiles)
{
	bool bNewPendingGameHotfix = false;
	for (int32 Idx = 0; Idx < PendingChangedFiles.Num(); Idx++)
	{
		if (PendingChangedFiles[Idx].FileName.EndsWith(TEXT("GAME.INI"), ESearchCase::IgnoreCase))
		{
			bNewPendingGameHotfix = true;
			break;
		}
	}

	if (bNewPendingGameHotfix && !bHasPendingGameHotfix)
	{
		bHasPendingGameHotfix = true;
		OnPendingGameHotfixChanged.Broadcast(bHasPendingGameHotfix);
	}
}

bool UUltraHotfixManager::PreProcessDownloadedFileData(TArray<uint8>& FileData) const
{
	// No need to preprocess if there is no data
	if (FileData.Num() == 0)
	{
		return true;
	}

	//TODO Any Preprocessing?

	return true;
}

#if !UE_BUILD_SHIPPING
void UUltraHotfixManager::GetOnScreenMessages(TMultiMap<FCoreDelegates::EOnScreenMessageSeverity, FText>& OutMessages)
{
	// TODO Any messages/errors.
}
#endif // !UE_BUILD_SHIPPING

void UUltraHotfixManager::RequestPatchAssetsFromIniFiles()
{
	if (!RequestPatchAssetsHandle.IsValid())
	{
		RequestPatchAssetsHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateWeakLambda(this, [this](float DeltaTime) {
			RequestPatchAssetsHandle.Reset();
			UE_LOG(LogHotfixManager, Display, TEXT("Hotfix manager re-calling PatchAssetsFromIniFiles due to new plugins"));
			PatchAssetsFromIniFiles();
			return false;
		}));
	}
}

void UUltraHotfixManager::StartHotfixProcess()
{
	if (GIsEditor)
	{
		UE_LOG(LogHotfixManager, Display, TEXT("Hotfixing skipped in development mode."));
		TriggerHotfixComplete(EHotfixResult::SuccessNoChange);
		return;
	}

#if ENABLE_SHARED_MEMORY_TRACKER
	FSharedMemoryTracker::PrintMemoryDiff(TEXT("StartHotfixProcess"));
#endif

	Super::StartHotfixProcess();
}

