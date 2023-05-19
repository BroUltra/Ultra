// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUltraCoreRuntime_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_UltraCoreRuntime;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_UltraCoreRuntime()
	{
		if (!Z_Registration_Info_UPackage__Script_UltraCoreRuntime.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/UltraCoreRuntime",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0x45D837D4,
				0xE9CAD248,
				METADATA_PARAMS(nullptr, 0)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_UltraCoreRuntime.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_UltraCoreRuntime.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_UltraCoreRuntime(Z_Construct_UPackage__Script_UltraCoreRuntime, TEXT("/Script/UltraCoreRuntime"), Z_Registration_Info_UPackage__Script_UltraCoreRuntime, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x45D837D4, 0xE9CAD248));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
