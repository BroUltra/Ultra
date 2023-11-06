// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraCharacterComponent.h"
#include "Components/GameFrameworkComponentDelegates.h"
#include "Logging/MessageLog.h"
#include "Input/UltraMappableConfigPair.h"
#include "UltraLogChannels.h"
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
#include "Components/GameFrameworkComponentManager.h"
#include "PlayerMappableInputConfig.h"
#include "Camera/UltraCameraMode.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "InputMappingContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraCharacterComponent)

#if WITH_EDITOR
#include "Misc/UObjectToken.h"
#endif	// WITH_EDITOR

namespace UltraCharacter
{
	static const float LookYawRate = 300.0f;
	static const float LookPitchRate = 165.0f;
};

const FName UUltraCharacterComponent::NAME_BindInputsNow("BindInputsNow");
const FName UUltraCharacterComponent::NAME_ActorFeatureName("Character");

UUltraCharacterComponent::UUltraCharacterComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilityCameraMode = nullptr;
	bReadyToBindInputs = false;
}

void UUltraCharacterComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogUltra, Error, TEXT("[UUltraCharacterComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));

#if WITH_EDITOR
		if (GIsEditor)
		{
			static const FText Message = NSLOCTEXT("UltraCharacterComponent", "NotOnPawnError", "has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint. This will cause a crash if you PIE!");
			static const FName CharacterMessageLogName = TEXT("UltraCharacterComponent");
			
			FMessageLog(CharacterMessageLogName).Error()
				->AddToken(FUObjectToken::Create(this, FText::FromString(GetNameSafe(this))))
				->AddToken(FTextToken::Create(Message));
				
			FMessageLog(CharacterMessageLogName).Open();
		}
#endif
	}
	else
	{
		// Register with the init state system early, this will only work if this is a game world
		RegisterInitStateFeature();
	}
}

bool UUltraCharacterComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);
	
	APawn* Pawn = GetPawn<APawn>();

	if (!CurrentState.IsValid() && DesiredState == UltraGameplayTags::InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == UltraGameplayTags::InitState_Spawned && DesiredState == UltraGameplayTags::InitState_DataAvailable)
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
	else if (CurrentState == UltraGameplayTags::InitState_DataAvailable && DesiredState == UltraGameplayTags::InitState_DataInitialized)
	{
		// Wait for player state and extension component
		AUltraPlayerState* UltraPS = GetPlayerState<AUltraPlayerState>();

		return UltraPS && Manager->HasFeatureReachedInitState(Pawn, UUltraPawnExtensionComponent::NAME_ActorFeatureName, UltraGameplayTags::InitState_DataInitialized);
	}
	else if (CurrentState == UltraGameplayTags::InitState_DataInitialized && DesiredState == UltraGameplayTags::InitState_GameplayReady)
	{
		// TODO add ability initialization checks?
		return true;
	}

	return false;
}

void UUltraCharacterComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (CurrentState == UltraGameplayTags::InitState_DataAvailable && DesiredState == UltraGameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AUltraPlayerState* UltraPS = GetPlayerState<AUltraPlayerState>();
		if (!ensure(Pawn && UltraPS))
		{
			return;
		}

		const UUltraPawnData* PawnData = nullptr;

		if (UUltraPawnExtensionComponent* PawnExtComp = UUltraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UUltraPawnData>();

			// The player state holds the persistent data for this player (state that persists across despawns and multiple pawns).
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

		// Hook up the delegate for all pawns, in case we spectate later
		if (PawnData)
		{
			if (UUltraCameraComponent* CameraComponent = UUltraCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
}

void UUltraCharacterComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UUltraPawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == UltraGameplayTags::InitState_DataInitialized)
		{
			// If the extension component says all all other components are initialized, try to progress to next state
			CheckDefaultInitialization();
		}
	}
}

void UUltraCharacterComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = { UltraGameplayTags::InitState_Spawned, UltraGameplayTags::InitState_DataAvailable, UltraGameplayTags::InitState_DataInitialized, UltraGameplayTags::InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

void UUltraCharacterComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for when the pawn extension component changes init state
	BindOnActorInitStateChanged(UUltraPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(UltraGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UUltraCharacterComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UUltraCharacterComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
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
				for (const FInputMappingContextAndPriority& Mapping : DefaultInputMappings)
				{
					if (UInputMappingContext* IMC = Mapping.InputMapping.Get())
					{
						if (Mapping.bRegisterWithSettings)
						{
							if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
							{
								Settings->RegisterInputMappingContext(IMC);
							}

							FModifyContextOptions Options = {};
							Options.bIgnoreAllPressedKeysUntilRelease = false;
							// Actually add the config to the local player							
							Subsystem->AddMappingContext(IMC, Mapping.Priority, Options);
						}
					}
				}
				
				// The Ultra Input Component has some additional functions to map Gameplay Tags to an Input Action.
				// If you want this functionality but still want to change your input component class, make it a subclass
				// of the UUltraInputComponent or modify this component accordingly.
				UUltraInputComponent* UltraIC = Cast<UUltraInputComponent>(PlayerInputComponent);
				if (ensureMsgf(UltraIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UUltraInputComponent or a subclass of it.")))
				{
					// Add the key mappings that may have been set by the player
					UltraIC->AddInputMappings(InputConfig, Subsystem);

					// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
					// be triggered directly by these input actions Triggered events. 
					TArray<uint32> BindHandles;
					UltraIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

					UltraIC->BindNativeAction(InputConfig, UltraGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
					UltraIC->BindNativeAction(InputConfig, UltraGameplayTags::InputTag_MoveVertical, ETriggerEvent::Triggered, this, &ThisClass::Input_MoveVertical, /*bLogIfNotFound=*/ false);
					UltraIC->BindNativeAction(InputConfig, UltraGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
					UltraIC->BindNativeAction(InputConfig, UltraGameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
					UltraIC->BindNativeAction(InputConfig, UltraGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
					UltraIC->BindNativeAction(InputConfig, UltraGameplayTags::InputTag_AutoRun, ETriggerEvent::Triggered, this, &ThisClass::Input_AutoRun, /*bLogIfNotFound=*/ false);
				}
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

void UUltraCharacterComponent::AddAdditionalInputConfig(const UUltraInputConfig* InputConfig)
{
	TArray<uint32> BindHandles;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	if (const UUltraPawnExtensionComponent* PawnExtComp = UUltraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		UUltraInputComponent* UltraIC = Pawn->FindComponentByClass<UUltraInputComponent>();
		if (ensureMsgf(UltraIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UUltraInputComponent or a subclass of it.")))
		{
			UltraIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
		}
	}
}

void UUltraCharacterComponent::RemoveAdditionalInputConfig(const UUltraInputConfig* InputConfig)
{
	//@TODO: Implement me!
}

bool UUltraCharacterComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

void UUltraCharacterComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
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

void UUltraCharacterComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
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

void UUltraCharacterComponent::Input_Move(const FInputActionValue& InputActionValue)
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
		const FRotator MovementRotation2D(Controller->GetControlRotation().Pitch, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation2D.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UUltraCharacterComponent::Input_MoveVertical(const FInputActionValue& InputActionValue)
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
		const FVector Value = InputActionValue.Get<FVector>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::UpVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}
	}
}

void UUltraCharacterComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
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

void UUltraCharacterComponent::Input_LookStick(const FInputActionValue& InputActionValue)
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
		Pawn->AddControllerYawInput(Value.X * UltraCharacter::LookYawRate * World->GetDeltaSeconds());
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y * UltraCharacter::LookPitchRate * World->GetDeltaSeconds());
	}
}

void UUltraCharacterComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (AUltraCharacter* Character = GetPawn<AUltraCharacter>())
	{
		Character->ToggleCrouch();
	}
}

void UUltraCharacterComponent::Input_AutoRun(const FInputActionValue& InputActionValue)
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

TSubclassOf<UUltraCameraMode> UUltraCharacterComponent::DetermineCameraMode() const
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

void UUltraCharacterComponent::SetAbilityCameraMode(TSubclassOf<UUltraCameraMode> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (CameraMode)
	{
		AbilityCameraMode = CameraMode;
		AbilityCameraModeOwningSpecHandle = OwningSpecHandle;
	}
}

void UUltraCharacterComponent::ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (AbilityCameraModeOwningSpecHandle == OwningSpecHandle)
	{
		AbilityCameraMode = nullptr;
		AbilityCameraModeOwningSpecHandle = FGameplayAbilitySpecHandle();
	}
}

