// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "NativeGameplayTags.h"

#include "UltraCharacterMovementComponent.generated.h"

class UObject;
struct FFrame;

ULTRAGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_MovementStopped);

UENUM(BlueprintType)
enum ECustomMovementMode : uint8
{
	CMOVE_None			UMETA(Hidden),
	CMOVE_CustomFly		UMETA(DisplayName = "Custom flight"),
	CMOVE_Dash			UMETA(DisplayName = "Dashing"),
	CMOVE_MAX			UMETA(Hidden),
};

/**
 * FUltraCharacterGroundInfo
 *
 *	Information about the ground under the character.  It only gets updated as needed.
 */
USTRUCT(BlueprintType)
struct FUltraCharacterGroundInfo
{
	GENERATED_BODY()

	FUltraCharacterGroundInfo()
		: LastUpdateFrame(0)
		, GroundDistance(0.0f)
	{}

	uint64 LastUpdateFrame;

	UPROPERTY(BlueprintReadOnly)
	FHitResult GroundHitResult;

	UPROPERTY(BlueprintReadOnly)
	float GroundDistance;
};


/**
 * UUltraCharacterMovementComponent
 *
 *	The base character movement component class used by this project.
 */
UCLASS(Config = Game)
class ULTRAGAME_API UUltraCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UUltraCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);

	virtual void SimulateMovement(float DeltaTime) override;

	virtual bool CanAttemptJump() const override;

	// Returns the current ground info.  Calling this will update the ground info if it's out of date.
	UFUNCTION(BlueprintCallable, Category = "Ultra|CharacterMovement")
	const FUltraCharacterGroundInfo& GetGroundInfo();

	void SetReplicatedAcceleration(const FVector& InAcceleration);

	//~UMovementComponent interface
	virtual FRotator GetDeltaRotation(float DeltaTime) const override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxAcceleration() const override;
	virtual float GetMaxBrakingDeceleration() const override;
	//~End of UMovementComponent interface

protected:

	virtual void InitializeComponent() override;

	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;
	

	////////// Custom movement //////////

		// CustomFly
	UPROPERTY(EditDefaultsOnly) float MaxCustomFlySpeed=1000.f;
	UPROPERTY(EditDefaultsOnly) float AccelerationCustomFly=10000.f;
	UPROPERTY(EditDefaultsOnly) float BrakingDecelerationCustomFly=10000.f;

		// Dash
	UPROPERTY(EditDefaultsOnly) float MaxDashSpeed=2000.f;
	UPROPERTY(EditDefaultsOnly) float AccelerationDash=20000.f;
	UPROPERTY(EditDefaultsOnly) float BrakingDecelerationDash=20000.f;
	
private:

	// Custom flight
	void PhysCustomFly(float DeltaTime, int32 Iterations);

	// Dashing
	void PhysDash(float DeltaTime, int32 Iterations);
	

protected:

	// Cached ground info for the character.  Do not access this directly!  It's only updated when accessed via GetGroundInfo().
	FUltraCharacterGroundInfo CachedGroundInfo;

	UPROPERTY(Transient)
	bool bHasReplicatedAcceleration = false;
};