// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraNumberPopComponent_NiagaraText.h"

#include "Containers/UnrealString.h"
#include "Feedback/NumberPops/UltraNumberPopComponent.h"
#include "GameFramework/Actor.h"
#include "Logging/LogCategory.h"
#include "Logging/LogMacros.h"
#include "UltraHitPopStyleNiagara.h"
#include "UltraLogChannels.h"
#include "Math/UnrealMathSSE.h"
#include "Math/Vector.h"
#include "Math/Vector4.h"
#include "Misc/AssertionMacros.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "Trace/Detail/Channel.h"
#include "UObject/ObjectPtr.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraNumberPopComponent_NiagaraText)

UUltraNumberPopComponent_NiagaraText::UUltraNumberPopComponent_NiagaraText(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{


}

void UUltraNumberPopComponent_NiagaraText::AddNumberPop(const FUltraNumberPopRequest& NewRequest)
{
	int32 LocalHit = NewRequest.NumberToDisplay;

	//Change Hit to negative to differentiate Critial vs Normal hit
	if (NewRequest.bIsCriticalHit)
	{
		LocalHit *= -1;
	}

	//Add a NiagaraComponent if we don't already have one
	if (!NiagaraComp)
	{
		NiagaraComp = NewObject<UNiagaraComponent>(GetOwner());
		if (Style != nullptr)
		{
			NiagaraComp->SetAsset(Style->TextNiagara);
			NiagaraComp->bAutoActivate = false;
			
		}
		NiagaraComp->SetupAttachment(nullptr);
		check(NiagaraComp);
		NiagaraComp->RegisterComponent();
	}


	NiagaraComp->Activate(false);
	NiagaraComp->SetWorldLocation(NewRequest.WorldLocation);

	UE_LOG(LogUltra, Log, TEXT("HitHit location : %s"), *(NewRequest.WorldLocation.ToString()));
	//Add Hit information to the current Niagara list - Hit information is packed inside a FVector4 where XYZ = Position, W = Hit
	TArray<FVector4> HitList = UNiagaraDataInterfaceArrayFunctionLibrary::GetNiagaraArrayVector4(NiagaraComp, Style->NiagaraArrayName);
	HitList.Add(FVector4(NewRequest.WorldLocation.X, NewRequest.WorldLocation.Y, NewRequest.WorldLocation.Z, LocalHit));
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector4(NiagaraComp, Style->NiagaraArrayName, HitList);
	
}

