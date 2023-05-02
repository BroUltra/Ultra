// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/UltraSimulatedInputWidget.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedPlayerInput.h"
#include "InputAction.h"
#include "UltraLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraSimulatedInputWidget)

#define LOCTEXT_NAMESPACE "UltraSimulatedInputWidget"

UUltraSimulatedInputWidget::UUltraSimulatedInputWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetConsumePointerInput(true);
}

#if WITH_EDITOR
const FText UUltraSimulatedInputWidget::GetPaletteCategory()
{
	return LOCTEXT("PalleteCategory", "Input");
}
#endif // WITH_EDITOR

void UUltraSimulatedInputWidget::NativeConstruct()
{
	// TODO: We should query the key the simulate whenever the user rebinds a key and after the PlayerController has
	// had it's input initalized. Doing it here will always result in the fallback key being used because
	// the PC does not have any keys mapped in enhanced input.
	QueryKeyToSimulate();
	
	Super::NativeConstruct();
}

FReply UUltraSimulatedInputWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FlushSimulatedInput();
	
	return Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
}

UEnhancedInputLocalPlayerSubsystem* UUltraSimulatedInputWidget::GetEnhancedInputSubsystem() const
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ULocalPlayer* LP = GetOwningLocalPlayer())
		{
			return LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		}
	}
	return nullptr;
}

UEnhancedPlayerInput* UUltraSimulatedInputWidget::GetPlayerInput() const
{
	if (UEnhancedInputLocalPlayerSubsystem* System = GetEnhancedInputSubsystem())
	{
		return System->GetPlayerInput();
	}
	return nullptr;
}

void UUltraSimulatedInputWidget::InputKeyValue(const FVector& Value)
{
	if (UEnhancedPlayerInput* Input = GetPlayerInput())
	{
		if(KeyToSimulate.IsValid())
		{
			FInputKeyParams Params;
			Params.Delta = Value;
			Params.Key = KeyToSimulate;
			Params.NumSamples = 1;
			Params.DeltaTime = GetWorld()->GetDeltaSeconds();
			Params.bIsGamepadOverride = KeyToSimulate.IsGamepadKey();
			
			Input->InputKey(Params);	
		}
	}
	else
	{
		UE_LOG(LogUltra, Error, TEXT("'%s' is attempting to simulate input but has no player input!"), *GetNameSafe(this));
	}
}

void UUltraSimulatedInputWidget::InputKeyValue2D(const FVector2D& Value)
{
	InputKeyValue(FVector(Value.X, Value.Y, 0.0));
}

void UUltraSimulatedInputWidget::FlushSimulatedInput()
{
	if (UEnhancedPlayerInput* Input = GetPlayerInput())
	{
		Input->FlushPressedKeys();
	}
}

void UUltraSimulatedInputWidget::QueryKeyToSimulate()
{
	if (UEnhancedInputLocalPlayerSubsystem* System = GetEnhancedInputSubsystem())
	{
		TArray<FKey> Keys = System->QueryKeysMappedToAction(AssociatedAction);
		if(!Keys.IsEmpty() && Keys[0].IsValid())
		{
			KeyToSimulate = Keys[0];
		}
		else
		{
			KeyToSimulate = FallbackBindingKey;
		}
	}
}

#undef LOCTEXT_NAMESPACE
