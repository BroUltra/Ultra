// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UltraCoreRuntime/Public/UltraCoreRuntimeSettings.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUltraCoreRuntimeSettings() {}
// Cross Module References
	DEVELOPERSETTINGS_API UClass* Z_Construct_UClass_UDeveloperSettings();
	ENGINE_API UEnum* Z_Construct_UEnum_Engine_ECollisionChannel();
	ULTRACORERUNTIME_API UClass* Z_Construct_UClass_UUltraCoreRuntimeSettings();
	ULTRACORERUNTIME_API UClass* Z_Construct_UClass_UUltraCoreRuntimeSettings_NoRegister();
	UPackage* Z_Construct_UPackage__Script_UltraCoreRuntime();
// End Cross Module References
	void UUltraCoreRuntimeSettings::StaticRegisterNativesUUltraCoreRuntimeSettings()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUltraCoreRuntimeSettings);
	UClass* Z_Construct_UClass_UUltraCoreRuntimeSettings_NoRegister()
	{
		return UUltraCoreRuntimeSettings::StaticClass();
	}
	struct Z_Construct_UClass_UUltraCoreRuntimeSettings_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AimAssistCollisionChannel_MetaData[];
#endif
		static const UECodeGen_Private::FBytePropertyParams NewProp_AimAssistCollisionChannel;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UUltraCoreRuntimeSettings_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UDeveloperSettings,
		(UObject* (*)())Z_Construct_UPackage__Script_UltraCoreRuntime,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUltraCoreRuntimeSettings_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UUltraCoreRuntimeSettings_Statics::Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Runtime settings specific to the UltraCoreRuntime plugin */" },
#endif
		{ "IncludePath", "UltraCoreRuntimeSettings.h" },
		{ "ModuleRelativePath", "Public/UltraCoreRuntimeSettings.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Runtime settings specific to the UltraCoreRuntime plugin" },
#endif
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UUltraCoreRuntimeSettings_Statics::NewProp_AimAssistCollisionChannel_MetaData[] = {
		{ "Category", "Aim Assist" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09 * What trace channel should be used to find available targets for Aim Assist.\n\x09 * @see UAimAssistTargetManagerComponent::GetVisibleTargets\n\x09 */" },
#endif
		{ "ModuleRelativePath", "Public/UltraCoreRuntimeSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "What trace channel should be used to find available targets for Aim Assist.\n@see UAimAssistTargetManagerComponent::GetVisibleTargets" },
#endif
	};
#endif
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_UUltraCoreRuntimeSettings_Statics::NewProp_AimAssistCollisionChannel = { "AimAssistCollisionChannel", nullptr, (EPropertyFlags)0x0040000000004001, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UUltraCoreRuntimeSettings, AimAssistCollisionChannel), Z_Construct_UEnum_Engine_ECollisionChannel, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUltraCoreRuntimeSettings_Statics::NewProp_AimAssistCollisionChannel_MetaData), Z_Construct_UClass_UUltraCoreRuntimeSettings_Statics::NewProp_AimAssistCollisionChannel_MetaData) }; // 1822723181
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UUltraCoreRuntimeSettings_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUltraCoreRuntimeSettings_Statics::NewProp_AimAssistCollisionChannel,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UUltraCoreRuntimeSettings_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUltraCoreRuntimeSettings>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UUltraCoreRuntimeSettings_Statics::ClassParams = {
		&UUltraCoreRuntimeSettings::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UUltraCoreRuntimeSettings_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UUltraCoreRuntimeSettings_Statics::PropPointers),
		0,
		0x000000A6u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUltraCoreRuntimeSettings_Statics::Class_MetaDataParams), Z_Construct_UClass_UUltraCoreRuntimeSettings_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUltraCoreRuntimeSettings_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_UUltraCoreRuntimeSettings()
	{
		if (!Z_Registration_Info_UClass_UUltraCoreRuntimeSettings.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUltraCoreRuntimeSettings.OuterSingleton, Z_Construct_UClass_UUltraCoreRuntimeSettings_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UUltraCoreRuntimeSettings.OuterSingleton;
	}
	template<> ULTRACORERUNTIME_API UClass* StaticClass<UUltraCoreRuntimeSettings>()
	{
		return UUltraCoreRuntimeSettings::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UUltraCoreRuntimeSettings);
	UUltraCoreRuntimeSettings::~UUltraCoreRuntimeSettings() {}
	struct Z_CompiledInDeferFile_FID_Users_UltraBro_Documents_Unreal_Projects_Ultra_5_3_Plugins_GameFeatures_UltraCore_Source_UltraCoreRuntime_Public_UltraCoreRuntimeSettings_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_UltraBro_Documents_Unreal_Projects_Ultra_5_3_Plugins_GameFeatures_UltraCore_Source_UltraCoreRuntime_Public_UltraCoreRuntimeSettings_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UUltraCoreRuntimeSettings, UUltraCoreRuntimeSettings::StaticClass, TEXT("UUltraCoreRuntimeSettings"), &Z_Registration_Info_UClass_UUltraCoreRuntimeSettings, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUltraCoreRuntimeSettings), 2269182464U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_UltraBro_Documents_Unreal_Projects_Ultra_5_3_Plugins_GameFeatures_UltraCore_Source_UltraCoreRuntime_Public_UltraCoreRuntimeSettings_h_2408936698(TEXT("/Script/UltraCoreRuntime"),
		Z_CompiledInDeferFile_FID_Users_UltraBro_Documents_Unreal_Projects_Ultra_5_3_Plugins_GameFeatures_UltraCore_Source_UltraCoreRuntime_Public_UltraCoreRuntimeSettings_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_UltraBro_Documents_Unreal_Projects_Ultra_5_3_Plugins_GameFeatures_UltraCore_Source_UltraCoreRuntime_Public_UltraCoreRuntimeSettings_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
