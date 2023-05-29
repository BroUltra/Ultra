// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraCharacter.h"

#include "AI/Navigation/NavigationTypes.h"
#include "AbilitySystem/UltraAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "Camera/UltraCameraComponent.h"
#include "Character/UltraDespawnComponent.h"
#include "Character/UltraPawnExtensionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Containers/EnumAsByte.h"
#include "Containers/Map.h"
#include "Containers/UnrealString.h"
#include "Delegates/Delegate.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameplayTagContainer.h"
#include "Logging/LogCategory.h"
#include "Logging/LogMacros.h"
#include "UltraCharacterMovementComponent.h"
#include "UltraGameplayTags.h"
#include "UltraLogChannels.h"
#include "Math/Rotator.h"
#include "Math/UnrealMathSSE.h"
#include "Math/Vector.h"
#include "Misc/AssertionMacros.h"
#include "Net/UnrealNetwork.h"
#include "Player/UltraPlayerController.h"
#include "Player/UltraPlayerState.h"
#include "SignificanceManager.h"
#include "System/UltraSignificanceManager.h"
#include "Templates/Casts.h"
#include "TimerManager.h"
#include "Trace/Detail/Channel.h"
#include "UObject/CoreNetTypes.h"
#include "UObject/NameTypes.h"
#include "UObject/Object.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectBaseUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraCharacter)

class AActor;
class FLifetimeProperty;
class IRepChangedPropertyTracker;
class UInputComponent;

static FName NAME_UltraCharacterCollisionProfile_Capsule(TEXT("UltraPawnCapsule"));
static FName NAME_UltraCharacterCollisionProfile_Mesh(TEXT("UltraPawnMesh"));

AUltraCharacter::AUltraCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UUltraCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Avoid ticking characters if possible.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	NetCullDistanceSquared = 900000000.0f;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);
	CapsuleComp->SetCollisionProfileName(NAME_UltraCharacterCollisionProfile_Capsule);

	USkeletalMeshComponent* MeshComp = GetMesh();
	check(MeshComp);
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));  // Rotate mesh to be X forward since it is exported as Y forward.
	MeshComp->SetCollisionProfileName(NAME_UltraCharacterCollisionProfile_Mesh);

	UUltraCharacterMovementComponent* UltraMoveComp = CastChecked<UUltraCharacterMovementComponent>(GetCharacterMovement());
	UltraMoveComp->GravityScale = 1.0f;
	UltraMoveComp->MaxAcceleration = 2400.0f;
	UltraMoveComp->BrakingFrictionFactor = 1.0f;
	UltraMoveComp->BrakingFriction = 6.0f;
	UltraMoveComp->GroundFriction = 8.0f;
	UltraMoveComp->BrakingDecelerationWalking = 1400.0f;
	UltraMoveComp->bUseControllerDesiredRotation = false;
	UltraMoveComp->bOrientRotationToMovement = false;
	UltraMoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	UltraMoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	UltraMoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	UltraMoveComp->bCanWalkOffLedgesWhenCrouching = true;
	UltraMoveComp->SetCrouchedHalfHeight(65.0f);

	PawnExtComponent = CreateDefaultSubobject<UUltraPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	PawnExtComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));

	DespawnComponent = CreateDefaultSubobject<UUltraDespawnComponent>(TEXT("DespawnComponent"));
	DespawnComponent->OnDespawnStarted.AddDynamic(this, &ThisClass::OnDespawnStarted);
	DespawnComponent->OnDespawnFinished.AddDynamic(this, &ThisClass::OnDespawnFinished);

	CameraComponent = CreateDefaultSubobject<UUltraCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	BaseEyeHeight = 80.0f;
	CrouchedEyeHeight = 50.0f;
}

void AUltraCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AUltraCharacter::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	const bool bRegisterWithSignificanceManager = !IsNetMode(NM_DedicatedServer);
	if (bRegisterWithSignificanceManager)
	{
		if (UUltraSignificanceManager* SignificanceManager = USignificanceManager::Get<UUltraSignificanceManager>(World))
		{
//@TODO: SignificanceManager->RegisterObject(this, (EFortSignificanceType)SignificanceType);
		}
	}
}

void AUltraCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UWorld* World = GetWorld();

	const bool bRegisterWithSignificanceManager = !IsNetMode(NM_DedicatedServer);
	if (bRegisterWithSignificanceManager)
	{
		if (UUltraSignificanceManager* SignificanceManager = USignificanceManager::Get<UUltraSignificanceManager>(World))
		{
			SignificanceManager->UnregisterObject(this);
		}
	}
}

void AUltraCharacter::Reset()
{
	DisableMovementAndCollision();

	K2_OnReset();

	UninitAndDestroy();
}

void AUltraCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, ReplicatedAcceleration, COND_SimulatedOnly);
	DOREPLIFETIME(ThisClass, MyTeamID)
}

void AUltraCharacter::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		// Compress Acceleration: XY components as direction + magnitude, Z component as direct value
		const double MaxAccel = MovementComponent->MaxAcceleration;
		const FVector CurrentAccel = MovementComponent->GetCurrentAcceleration();
		double AccelXYRadians, AccelXYMagnitude;
		FMath::CartesianToPolar(CurrentAccel.X, CurrentAccel.Y, AccelXYMagnitude, AccelXYRadians);

		ReplicatedAcceleration.AccelXYRadians   = FMath::FloorToInt((AccelXYRadians / TWO_PI) * 255.0);     // [0, 2PI] -> [0, 255]
		ReplicatedAcceleration.AccelXYMagnitude = FMath::FloorToInt((AccelXYMagnitude / MaxAccel) * 255.0);	// [0, MaxAccel] -> [0, 255]
		ReplicatedAcceleration.AccelZ           = FMath::FloorToInt((CurrentAccel.Z / MaxAccel) * 127.0);   // [-MaxAccel, MaxAccel] -> [-127, 127]
	}
}

void AUltraCharacter::NotifyControllerChanged()
{
	const FGenericTeamId OldTeamId = GetGenericTeamId();

	Super::NotifyControllerChanged();

	// Update our team ID based on the controller
	if (HasAuthority() && (Controller != nullptr))
	{
		if (IUltraTeamAgentInterface* ControllerWithTeam = Cast<IUltraTeamAgentInterface>(Controller))
		{
			MyTeamID = ControllerWithTeam->GetGenericTeamId();
			ConditionalBroadcastTeamChanged(this, OldTeamId, MyTeamID);
		}
	}
}

AUltraPlayerController* AUltraCharacter::GetUltraPlayerController() const
{
	return CastChecked<AUltraPlayerController>(Controller, ECastCheckedType::NullAllowed);
}

AUltraPlayerState* AUltraCharacter::GetUltraPlayerState() const
{
	return CastChecked<AUltraPlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

UUltraAbilitySystemComponent* AUltraCharacter::GetUltraAbilitySystemComponent() const
{
	return Cast<UUltraAbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* AUltraCharacter::GetAbilitySystemComponent() const
{
	return PawnExtComponent->GetUltraAbilitySystemComponent();
}

void AUltraCharacter::OnAbilitySystemInitialized()
{
	UUltraAbilitySystemComponent* UltraASC = GetUltraAbilitySystemComponent();
	check(UltraASC);

	DespawnComponent->InitializeWithAbilitySystem(UltraASC);

	InitializeGameplayTags();
}

void AUltraCharacter::OnAbilitySystemUninitialized()
{
	DespawnComponent->UninitializeFromAbilitySystem();
}

void AUltraCharacter::PossessedBy(AController* NewController)
{
	const FGenericTeamId OldTeamID = MyTeamID;

	Super::PossessedBy(NewController);

	PawnExtComponent->HandleControllerChanged();

	// Grab the current team ID and listen for future changes
	if (IUltraTeamAgentInterface* ControllerAsTeamProvider = Cast<IUltraTeamAgentInterface>(NewController))
	{
		MyTeamID = ControllerAsTeamProvider->GetGenericTeamId();
		ControllerAsTeamProvider->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnControllerChangedTeam);
	}
	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void AUltraCharacter::UnPossessed()
{
	AController* const OldController = Controller;

	// Stop listening for changes from the old controller
	const FGenericTeamId OldTeamID = MyTeamID;
	if (IUltraTeamAgentInterface* ControllerAsTeamProvider = Cast<IUltraTeamAgentInterface>(OldController))
	{
		ControllerAsTeamProvider->GetTeamChangedDelegateChecked().RemoveAll(this);
	}

	Super::UnPossessed();

	PawnExtComponent->HandleControllerChanged();

	// Determine what the new team ID should be afterwards
	MyTeamID = DetermineNewTeamAfterPossessionEnds(OldTeamID);
	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void AUltraCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	PawnExtComponent->HandleControllerChanged();
}

void AUltraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	PawnExtComponent->HandlePlayerStateReplicated();
}

void AUltraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComponent->SetupPlayerInputComponent();
}

void AUltraCharacter::InitializeGameplayTags()
{
	// Clear tags that may be lingering on the ability system from the previous pawn.
	if (UUltraAbilitySystemComponent* UltraASC = GetUltraAbilitySystemComponent())
	{
		const FUltraGameplayTags& GameplayTags = FUltraGameplayTags::Get();

		for (const TPair<uint8, FGameplayTag>& TagMapping : GameplayTags.MovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				UltraASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		for (const TPair<uint8, FGameplayTag>& TagMapping : GameplayTags.CustomMovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				UltraASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		UUltraCharacterMovementComponent* UltraMoveComp = CastChecked<UUltraCharacterMovementComponent>(GetCharacterMovement());
		SetMovementModeTag(UltraMoveComp->MovementMode, UltraMoveComp->CustomMovementMode, true);
	}
}

void AUltraCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const UUltraAbilitySystemComponent* UltraASC = GetUltraAbilitySystemComponent())
	{
		UltraASC->GetOwnedGameplayTags(TagContainer);
	}
}

bool AUltraCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if (const UUltraAbilitySystemComponent* UltraASC = GetUltraAbilitySystemComponent())
	{
		return UltraASC->HasMatchingGameplayTag(TagToCheck);
	}

	return false;
}

bool AUltraCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UUltraAbilitySystemComponent* UltraASC = GetUltraAbilitySystemComponent())
	{
		return UltraASC->HasAllMatchingGameplayTags(TagContainer);
	}

	return false;
}

bool AUltraCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UUltraAbilitySystemComponent* UltraASC = GetUltraAbilitySystemComponent())
	{
		return UltraASC->HasAnyMatchingGameplayTags(TagContainer);
	}

	return false;
}

void AUltraCharacter::OnDespawnStarted(AActor*)
{
	DisableMovementAndCollision();
}

void AUltraCharacter::OnDespawnFinished(AActor*)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDespawn);
}


void AUltraCharacter::DisableMovementAndCollision()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	UUltraCharacterMovementComponent* UltraMoveComp = CastChecked<UUltraCharacterMovementComponent>(GetCharacterMovement());
	UltraMoveComp->StopMovementImmediately();
	UltraMoveComp->DisableMovement();
}

void AUltraCharacter::DestroyDueToDespawn()
{
	K2_OnDespawnFinished();

	UninitAndDestroy();
}


void AUltraCharacter::UninitAndDestroy()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(0.1f);
	}

	// Uninitialize the ASC if we're still the avatar actor (otherwise another pawn already did it when they became the avatar actor)
	if (UUltraAbilitySystemComponent* UltraASC = GetUltraAbilitySystemComponent())
	{
		if (UltraASC->GetAvatarActor() == this)
		{
			PawnExtComponent->UninitializeAbilitySystem();
		}
	}

	SetActorHiddenInGame(true);
}

void AUltraCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	UUltraCharacterMovementComponent* UltraMoveComp = CastChecked<UUltraCharacterMovementComponent>(GetCharacterMovement());

	SetMovementModeTag(PrevMovementMode, PreviousCustomMode, false);
	SetMovementModeTag(UltraMoveComp->MovementMode, UltraMoveComp->CustomMovementMode, true);
}

void AUltraCharacter::SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled)
{
	if (UUltraAbilitySystemComponent* UltraASC = GetUltraAbilitySystemComponent())
	{
		const FUltraGameplayTags& GameplayTags = FUltraGameplayTags::Get();
		const FGameplayTag* MovementModeTag = nullptr;

		if (MovementMode == MOVE_Custom)
		{
			MovementModeTag = GameplayTags.CustomMovementModeTagMap.Find(CustomMovementMode);
		}
		else
		{
			MovementModeTag = GameplayTags.MovementModeTagMap.Find(MovementMode);
		}

		if (MovementModeTag && MovementModeTag->IsValid())
		{
			UltraASC->SetLooseGameplayTagCount(*MovementModeTag, (bTagEnabled ? 1 : 0));
		}
	}
}

void AUltraCharacter::ToggleCrouch()
{
	const UUltraCharacterMovementComponent* UltraMoveComp = CastChecked<UUltraCharacterMovementComponent>(GetCharacterMovement());

	if (bIsCrouched || UltraMoveComp->bWantsToCrouch)
	{
		UnCrouch();
	}
	else if (UltraMoveComp->IsMovingOnGround())
	{
		Crouch();
	}
}

void AUltraCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (UUltraAbilitySystemComponent* UltraASC = GetUltraAbilitySystemComponent())
	{
		UltraASC->SetLooseGameplayTagCount(FUltraGameplayTags::Get().Status_Crouching, 1);
	}
	
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void AUltraCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (UUltraAbilitySystemComponent* UltraASC = GetUltraAbilitySystemComponent())
	{
		UltraASC->SetLooseGameplayTagCount(FUltraGameplayTags::Get().Status_Crouching, 0);
	}

	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

bool AUltraCharacter::CanJumpInternal_Implementation() const
{
	// same as ACharacter's implementation but without the crouch check
	return JumpIsAllowedInternal();
}

void AUltraCharacter::OnRep_ReplicatedAcceleration()
{
	if (UUltraCharacterMovementComponent* UltraMovementComponent = Cast<UUltraCharacterMovementComponent>(GetCharacterMovement()))
	{
		// Decompress Acceleration
		const double MaxAccel         = UltraMovementComponent->MaxAcceleration;
		const double AccelXYMagnitude = double(ReplicatedAcceleration.AccelXYMagnitude) * MaxAccel / 255.0; // [0, 255] -> [0, MaxAccel]
		const double AccelXYRadians   = double(ReplicatedAcceleration.AccelXYRadians) * TWO_PI / 255.0;     // [0, 255] -> [0, 2PI]

		FVector UnpackedAcceleration(FVector::ZeroVector);
		FMath::PolarToCartesian(AccelXYMagnitude, AccelXYRadians, UnpackedAcceleration.X, UnpackedAcceleration.Y);
		UnpackedAcceleration.Z = double(ReplicatedAcceleration.AccelZ) * MaxAccel / 127.0; // [-127, 127] -> [-MaxAccel, MaxAccel]

		UltraMovementComponent->SetReplicatedAcceleration(UnpackedAcceleration);
	}
}

void AUltraCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (GetController() == nullptr)
	{
		if (HasAuthority())
		{
			const FGenericTeamId OldTeamID = MyTeamID;
			MyTeamID = NewTeamID;
			ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
		}
		else
		{
			UE_LOG(LogUltraTeams, Error, TEXT("You can't set the team ID on a character (%s) except on the authority"), *GetPathNameSafe(this));
		}
	}
	else
	{
		UE_LOG(LogUltraTeams, Error, TEXT("You can't set the team ID on a possessed character (%s); it's driven by the associated controller"), *GetPathNameSafe(this));
	}
}

FGenericTeamId AUltraCharacter::GetGenericTeamId() const
{
	return MyTeamID;
}

FOnUltraTeamIndexChangedDelegate* AUltraCharacter::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

void AUltraCharacter::OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	const FGenericTeamId MyOldTeamID = MyTeamID;
	MyTeamID = IntegerToGenericTeamId(NewTeam);
	ConditionalBroadcastTeamChanged(this, MyOldTeamID, MyTeamID);
}

void AUltraCharacter::OnRep_MyTeamID(FGenericTeamId OldTeamID)
{
	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}