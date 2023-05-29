// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UltraCoreRuntime/Private/Battle_PlayerSpawningManagementComponent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBattle_PlayerSpawningManagementComponent() {}
// Cross Module References
	ULTRACORERUNTIME_API UClass* Z_Construct_UClass_UBattle_PlayerSpawningManagementComponent();
	ULTRACORERUNTIME_API UClass* Z_Construct_UClass_UBattle_PlayerSpawningManagementComponent_NoRegister();
	ULTRAGAME_API UClass* Z_Construct_UClass_UUltraPlayerSpawningManagerComponent();
	UPackage* Z_Construct_UPackage__Script_UltraCoreRuntime();
// End Cross Module References
	void UBattle_PlayerSpawningManagementComponent::StaticRegisterNativesUBattle_PlayerSpawningManagementComponent()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBattle_PlayerSpawningManagementComponent);
	UClass* Z_Construct_UClass_UBattle_PlayerSpawningManagementComponent_NoRegister()
	{
		return UBattle_PlayerSpawningManagementComponent::StaticClass();
	}
	struct Z_Construct_UClass_UBattle_PlayerSpawningManagementComponent_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UBattle_PlayerSpawningManagementComponent_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UUltraPlayerSpawningManagerComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_UltraCoreRuntime,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UBattle_PlayerSpawningManagementComponent_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Trigger PhysicsVolume" },
		{ "IncludePath", "Battle_PlayerSpawningManagementComponent.h" },
		{ "ModuleRelativePath", "Private/Battle_PlayerSpawningManagementComponent.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UBattle_PlayerSpawningManagementComponent_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBattle_PlayerSpawningManagementComponent>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UBattle_PlayerSpawningManagementComponent_Statics::ClassParams = {
		&UBattle_PlayerSpawningManagementComponent::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x00A000A4u,
		METADATA_PARAMS(Z_Construct_UClass_UBattle_PlayerSpawningManagementComponent_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UBattle_PlayerSpawningManagementComponent_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UBattle_PlayerSpawningManagementComponent()
	{
		if (!Z_Registration_Info_UClass_UBattle_PlayerSpawningManagementComponent.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBattle_PlayerSpawningManagementComponent.OuterSingleton, Z_Construct_UClass_UBattle_PlayerSpawningManagementComponent_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UBattle_PlayerSpawningManagementComponent.OuterSingleton;
	}
	template<> ULTRACORERUNTIME_API UClass* StaticClass<UBattle_PlayerSpawningManagementComponent>()
	{
		return UBattle_PlayerSpawningManagementComponent::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UBattle_PlayerSpawningManagementComponent);
	UBattle_PlayerSpawningManagementComponent::~UBattle_PlayerSpawningManagementComponent() {}
	struct Z_CompiledInDeferFile_FID_Users_UltraBro_Documents_Unreal_Projects_Ultra_5_2_Plugins_GameFeatures_UltraCore_Source_UltraCoreRuntime_Private_Battle_PlayerSpawningManagementComponent_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_UltraBro_Documents_Unreal_Projects_Ultra_5_2_Plugins_GameFeatures_UltraCore_Source_UltraCoreRuntime_Private_Battle_PlayerSpawningManagementComponent_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UBattle_PlayerSpawningManagementComponent, UBattle_PlayerSpawningManagementComponent::StaticClass, TEXT("UBattle_PlayerSpawningManagementComponent"), &Z_Registration_Info_UClass_UBattle_PlayerSpawningManagementComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBattle_PlayerSpawningManagementComponent), 2100924136U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_UltraBro_Documents_Unreal_Projects_Ultra_5_2_Plugins_GameFeatures_UltraCore_Source_UltraCoreRuntime_Private_Battle_PlayerSpawningManagementComponent_h_420263816(TEXT("/Script/UltraCoreRuntime"),
		Z_CompiledInDeferFile_FID_Users_UltraBro_Documents_Unreal_Projects_Ultra_5_2_Plugins_GameFeatures_UltraCore_Source_UltraCoreRuntime_Private_Battle_PlayerSpawningManagementComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_UltraBro_Documents_Unreal_Projects_Ultra_5_2_Plugins_GameFeatures_UltraCore_Source_UltraCoreRuntime_Private_Battle_PlayerSpawningManagementComponent_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
