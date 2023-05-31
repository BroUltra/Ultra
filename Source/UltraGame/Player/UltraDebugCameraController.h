// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DebugCameraController.h"

#include "UltraDebugCameraController.generated.h"

class UObject;


/**
 * AUltraDebugCameraController
 *
 *	Used for controlling the debug camera when it is enabled via the cheat manager.
 */
UCLASS()
class AUltraDebugCameraController : public ADebugCameraController
{
	GENERATED_BODY()

public:

	AUltraDebugCameraController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void AddCheats(bool bForce) override;
};
