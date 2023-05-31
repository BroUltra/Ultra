// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "UltraVerbMessage.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "UltraVerbMessageReplication.generated.h"

class UObject;
struct FUltraVerbMessageReplication;
struct FNetDeltaSerializeInfo;

/**
 * Represents one verb message
 */
USTRUCT(BlueprintType)
struct FUltraVerbMessageReplicationEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FUltraVerbMessageReplicationEntry()
	{}

	FUltraVerbMessageReplicationEntry(const FUltraVerbMessage& InMessage)
		: Message(InMessage)
	{
	}

	FString GetDebugString() const;

private:
	friend FUltraVerbMessageReplication;

	UPROPERTY()
	FUltraVerbMessage Message;
};

/** Container of verb messages to replicate */
USTRUCT(BlueprintType)
struct FUltraVerbMessageReplication : public FFastArraySerializer
{
	GENERATED_BODY()

	FUltraVerbMessageReplication()
	{
	}

public:
	void SetOwner(UObject* InOwner) { Owner = InOwner; }

	// Broadcasts a message from server to clients
	void AddMessage(const FUltraVerbMessage& Message);

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FUltraVerbMessageReplicationEntry, FUltraVerbMessageReplication>(CurrentMessages, DeltaParms, *this);
	}

private:
	void RebroadcastMessage(const FUltraVerbMessage& Message);

private:
	// Replicated list of gameplay tag stacks
	UPROPERTY()
	TArray<FUltraVerbMessageReplicationEntry> CurrentMessages;
	
	// Owner (for a route to a world)
	UPROPERTY()
	TObjectPtr<UObject> Owner = nullptr;
};

template<>
struct TStructOpsTypeTraits<FUltraVerbMessageReplication> : public TStructOpsTypeTraitsBase2<FUltraVerbMessageReplication>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};
