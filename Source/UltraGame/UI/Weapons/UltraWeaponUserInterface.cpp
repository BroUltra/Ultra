// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraWeaponUserInterface.h"

#include "Equipment/UltraEquipmentManagerComponent.h"
#include "GameFramework/Pawn.h"
#include "Weapons/UltraWeaponInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraWeaponUserInterface)

struct FGeometry;

UUltraWeaponUserInterface::UUltraWeaponUserInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUltraWeaponUserInterface::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUltraWeaponUserInterface::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUltraWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		if (UUltraEquipmentManagerComponent* EquipmentManager = Pawn->FindComponentByClass<UUltraEquipmentManagerComponent>())
		{
			if (UUltraWeaponInstance* NewInstance = EquipmentManager->GetFirstInstanceOfType<UUltraWeaponInstance>())
			{
				if (NewInstance != CurrentInstance && NewInstance->GetInstigator() != nullptr)
				{
					UUltraWeaponInstance* OldWeapon = CurrentInstance;
					CurrentInstance = NewInstance;
					RebuildWidgetFromWeapon();
					OnWeaponChanged(OldWeapon, CurrentInstance);
				}
			}
		}
	}
}

void UUltraWeaponUserInterface::RebuildWidgetFromWeapon()
{
	
}

