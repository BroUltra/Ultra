// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraContextEffectsLibraryFactory.h"

#include "Feedback/ContextEffects/UltraContextEffectsLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UltraContextEffectsLibraryFactory)

class FFeedbackContext;
class UClass;
class UObject;

UUltraContextEffectsLibraryFactory::UUltraContextEffectsLibraryFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UUltraContextEffectsLibrary::StaticClass();

	bCreateNew = true;
	bEditorImport = false;
	bEditAfterNew = true;
}

UObject* UUltraContextEffectsLibraryFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UUltraContextEffectsLibrary* UltraContextEffectsLibrary = NewObject<UUltraContextEffectsLibrary>(InParent, Name, Flags);

	return UltraContextEffectsLibrary;
}
