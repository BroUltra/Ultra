// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


/**
 * when you modify this, please note that this information can be saved with instances
 * also DefaultEngine.ini [/Script/Engine.CollisionProfile] should match with this list
 **/

// Trace against Actors/Components which provide interactions.
#define Ultra_TraceChannel_Interaction					ECC_GameTraceChannel1

// Trace used by attacks, will hit physics assets instead of capsules
#define Ultra_TraceChannel_Attack						ECC_GameTraceChannel2

// Trace used by by attacks, will hit pawn capsules instead of physics assets
#define Ultra_TraceChannel_Attack_Capsule				ECC_GameTraceChannel3

// Trace used by by attacks, will trace through multiple pawns rather than stopping on the first hit
#define Ultra_TraceChannel_Attack_Multi					ECC_GameTraceChannel4
