// Copyright Epic Games, Inc.All Rights Reserved.

#include "Tests/UltraTestControllerBootTest.h"

#include "HAL/PlatformTime.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraTestControllerBootTest)

bool UUltraTestControllerBootTest::IsBootProcessComplete() const
{
	static double StartTime = FPlatformTime::Seconds();
	const double TimeSinceStart = FPlatformTime::Seconds() - StartTime;

	if (TimeSinceStart >= TestDelay)
	{
		return true;
//@TODO: actually do some useful testing here
// 		if (const UWorld* World = GetWorld())
// 		{
// 			if (const UUltraGameInstance* GameInstance = Cast<UUltraGameInstance>(GetWorld()->GetGameInstance()))
// 			{
// 				if (GameInstance->GetCurrentState() == ShooterGameInstanceState::WelcomeScreen ||
// 					GameInstance->GetCurrentState() == ShooterGameInstanceState::MainMenu)
// 				{
// 					return true;
// 				}
// 			}
// 		}
	}

	return false;
}
