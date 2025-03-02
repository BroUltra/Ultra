// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonGameInstance.h"

#include "UltraGameInstance.generated.h"

class AUltraPlayerController;
class UObject;

UCLASS(Config = Game)
class ULTRAGAME_API UUltraGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()

public:

	UUltraGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	AUltraPlayerController* GetPrimaryPlayerController() const;
	
	virtual bool CanJoinRequestedSession() const override;

	virtual void ReceivedNetworkEncryptionToken(const FString& EncryptionToken, const FOnEncryptionKeyResponse& Delegate) override;
	virtual void ReceivedNetworkEncryptionAck(const FOnEncryptionKeyResponse& Delegate) override;

protected:

	virtual void Init() override;
	virtual void Shutdown() override;

	void OnPreClientTravelToSession(FString& URL);

	/** A hard-coded encryption key used to try out the encryption code. This is NOT SECURE, do not use this technique in production! */
	TArray<uint8> DebugTestEncryptionKey;
};
