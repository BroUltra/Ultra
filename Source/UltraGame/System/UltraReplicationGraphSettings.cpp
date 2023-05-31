// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraReplicationGraphSettings.h"
#include "Misc/App.h"
#include "System/UltraReplicationGraph.h"

UUltraReplicationGraphSettings::UUltraReplicationGraphSettings()
{
	CategoryName = TEXT("Game");
	DefaultReplicationGraphClass = UUltraReplicationGraph::StaticClass();
}