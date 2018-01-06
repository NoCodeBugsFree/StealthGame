// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "FPSGameState.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(class APawn* InstigatorPawn, bool bMisiionSuccess)
{
	if(AFPSGameState* FPSGameState = GetGameState<AFPSGameState>())
	{
		FPSGameState->MulticastOnMissionComplete(InstigatorPawn, bMisiionSuccess);
	}
}