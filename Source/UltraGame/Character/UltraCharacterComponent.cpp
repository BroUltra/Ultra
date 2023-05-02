// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraHeroComponent.h"
#include "UltraLogChannels.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/UltraPlayerController.h"
#include "Player/UltraPlayerState.h"
#include "Player/UltraLocalPlayer.h"
#include "Character/UltraPawnExtensionComponent.h"
#include "Character/UltraPawnData.h"
#include "Character/UltraCharacter.h"
#include "AbilitySystem/UltraAbilitySystemComponent.h"
#include "Input/UltraInputConfig.h"
#include "Input/UltraInputComponent.h"
#include "Camera/UltraCameraComponent.h"
#include "UltraGameplayTags.h"
#include "Engine/LocalPlayer.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Settings/UltraSettingsLocal.h"
#include "System/UltraAssetManager.h"
#include "PlayerMappableInputConfig.h"
#include "Camera/UltraCameraMode.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraHeroComponent)

#if WITH_EDITOR
#include "Misc/UObjectToken.h"
#endif	// WITH_EDITOR

namespace UltraHero
{
	static const float LookYawRate = 300.0f;
	static const float LookPitchRate = 165.0f;
};

const FName UUltraHeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName UUltraHeroComponent::NAME_ActorFeatureName("Hero");

UUltraHeroComponent::UUltraHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilityCameraMode = nullptr;
	bReadyToBindInputs = false;
}

void UUltraHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogUltra, Error, TEXT("[UUltraHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));

#if WITH_EDITOR
		if (GIsEditor)
		{
			static const FText Message = NSLOCTEXT("UltraHeroComponent", "NotOnPawnError", "has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint. This will cause a crash if you PIE!");
			static const FName HeroMessageLogName = TEXT("UltraHeroComponent");
			
			FMessageLog(HeroMessageLogName).Error()
				->AddToken(FUObjectToken::Create(this, FText::FromString(GetNameSafe(this))))
				->AddToken(FTextToken::Create(Message));
				
			FMessageLog(HeroMessageLogName).Open();
		}
#endif
	}
	else
	{
		// Register with the init state system early, this will only work if this is a game world
		RegisterInitStateFeature();
	}
}

bool UUltraHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	const FUltraGameplayTags& InitTags = FUltraGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();

	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		// The player state is required.
		if (!GetPlayerState<AUltraPlayerState>())
		{
			return false;
		}

		// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();

			const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
				(Controller->PlayerState != nullptr) && \
				(Controller->PlayerState->GetOwner() == Controller);

			if (!bHasControllerPairedWithPS)
			{
				return false;
			}
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();

		if (bIsLocallyControlled && !bIsBot)
		{
			AUltraPlayerController* UltraPC = GetController<AUltraPlayerController>();

			// The input component and local player is required when locally controlled.
			if (!Pawn->InputComponent || !UltraPC || !UltraPC->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// Wait for player state and extension component
		AUltraPlayerState* UltraPS = GetPlayerState<AUltraPlayerState>();

		return UltraPS && Manager->HasFeatureReachedInitState(Pawn, UUltraPawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}
	else if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		// TODO add ability initialization checks?
		return true;
	}

	return false;
}

void UUltraHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	const FUltraGameplayTags& InitTags = FUltraGameplayTags::Get();
	if (CurrentState == FUltraGameplayTags::Get().InitState_DataAvailable && DesiredState == FUltraGameplayTags::Get().InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AUltraPlayerState* UltraPS = GetPlayerState<AUltraPlayerState>();
		if (!ensure(Pawn && UltraPS))
		{
			return;
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const UUltraPawnData* PawnData = nullptr;

		if (UUltraPawnExtensionComponent* PawnExtComp = UUltraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UUltraPawnData>();

			// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
			// The ability system component and attribute sets live on the player state.
			PawnExtComp->InitializeAbilitySystem(UltraPS->GetUltraAbilitySystemComponent(), UltraPS);
		}

		if (AUltraPlayerController* UltraPC = GetController<AUltraPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}

		if (bIsLocallyControlled && PawnData)
		{
			if (UUltraCameraComponent* CameraComponent = UUltraCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
}

void UUltraHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UUltraPawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == FUltraGameplayTags::Get().InitState_DataInitialized)
		{
			// If the extension component says all all other components are initialized, try to progress to next state
			CheckDefaultInitialization();
		}
	}
}

void UUltraHeroComponent::CheckDefaultInitialization()
{
	const FUltraGameplayTags& InitTags = FUltraGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

void UUltraHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for when the pawn extension component changes init state
	BindOnActorInitStateChanged(UUltraPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(FUltraGameplayTags::Get().InitState_Spawned));
	CheckDefaultInitialization();
}

void UUltraHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UUltraHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const UUltraLocalPlayer* LP = Cast<UUltraLocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if (const UUltraPawnExtensionComponent* PawnExtComp = UUltraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UUltraPawnData* PawnData = PawnExtComp->GetPawnData<UUltraPawnData>())
		{
			if (const UUltraInputConfig* InputConfig = PawnData->InputConfig)
			{
				const FUltraGameplayTags& GameplayTags = FUltraGameplayTags::Get();
	
				// Register any default input configs with the settings so that they will be applied to the player during AddInputMappings
				for (const FMappableConfigPair& Pair : DefaultInputConfigs)
				{
					if (Pair.bShouldActivateAutomatically && Pair.CanBeActivated())
					{
						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;
						// Actually add the config to the local player							
						Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);	
					}
				}
				
				UUltraInputComponent* UltraIC = CastChecked<UUltraInputComponent>(PlayerInputComponent);
				UltraIC->AddInputMappings(InputConfig, Subsystem);

				TArray<uint32> BindHandles;
				UltraIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

				UltraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
				UltraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
				UltraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
				UltraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
				UltraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_AutoRun, ETriggerEvent::Triggered, this, &ThisClass::Input_AutoRun, /*bLogIfNotFound=*/ false);
			}
		}
	}

	if (ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}
 
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void UUltraHeroComponent::AddAdditionalInputConfig(const UUltraInputConfig* InputConfig)
{
	TArray<uint32> BindHandles;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	UUltraInputComponent* UltraIC = Pawn->FindComponentByClass<UUltraInputComponent>();
	check(UltraIC);

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	if (const UUltraPawnExtensionComponent* PawnExtComp = UUltraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		UltraIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
	}
}

void UUltraHeroComponent::RemoveAdditionalInputConfig(const UUltraInputConfig* InputConfig)
{
	//@TODO: Implement me!
}

bool UUltraHeroComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

void UUltraHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const UUltraPawnExtensionComponent* PawnExtComp = UUltraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (UUltraAbilitySystemComponent* UltraASC = PawnExtComp->GetUltraAbilitySystemComponent())
			{
				UltraASC->AbilityInputTagPressed(InputTag);
			}
		}	
	}
}

void UUltraHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	if (const UUltraPawnExtensionComponent* PawnExtComp = UUltraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (UUltraAbilitySystemComponent* UltraASC = PawnExtComp->GetUltraAbilitySystemComponent())
		{
			UltraASC->AbilityInputTagReleased(InputTag);
		}
	}
}

void UUltraHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	// If the player has attempted to move again then cancel auto running
	if (AUltraPlayerController* UltraController = Cast<AUltraPlayerController>(Controller))
	{
		UltraController->SetIsAutoRunning(false);
	}
	
	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UUltraHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void UUltraHeroComponent::Input_LookStick(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	const UWorld* World = GetWorld();
	check(World);

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X * UltraHero::LookYawRate * World->GetDeltaSeconds());
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y * UltraHero::LookPitchRate * World->GetDeltaSeconds());
	}
}

void UUltraHeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (AUltraCharacter* Character = GetPawn<AUltraCharacter>())
	{
		Character->ToggleCrouch();
	}
}

void UUltraHeroComponent::Input_AutoRun(const FInputActionValue& InputActionValue)
{
	if (APawn* Pawn = GetPawn<APawn>())
	{
		if (AUltraPlayerController* Controller = Cast<AUltraPlayerController>(Pawn->GetController()))
		{
			// Toggle auto running
			Controller->SetIsAutoRunning(!Controller->GetIsAutoRunning());
		}	
	}
}

TSubclassOf<UUltraCameraMode> UUltraHeroComponent::DetermineCameraMode() const
{
	if (AbilityCameraMode)
	{
		return AbilityCameraMode;
	}

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (UUltraPawnExtensionComponent* PawnExtComp = UUltraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UUltraPawnData* PawnData = PawnExtComp->GetPawnData<UUltraPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}

void UUltraHeroComponent::SetAbilityCameraMode(TSubclassOf<UUltraCameraMode> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (CameraMode)
	{
		AbilityCameraMode = CameraMode;
		AbilityCameraModeOwningSpecHandle = OwningSpecHandle;
	}
}

void UUltraHeroComponent::ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (AbilityCameraModeOwningSpecHandle == OwningSpecHandle)
	{
		AbilityCameraMode = nullptr;
		AbilityCameraModeOwningSpecHandle = FGameplayAbilitySpecHandle();
	}
}

