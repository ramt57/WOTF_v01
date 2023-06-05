// Copyright Epic Games, Inc. All Rights Reserved.


#include "WOTFGameModeBase.h"

#include "GameState/WOTFGameState.h"
#include "HUD/WOTFHudBase.h"
#include "PlayerController/WOTFPlayerControllerBase.h"
#include "PlayerState/WOTFPlayerState.h"
#include "utils/FLogUtil.h"


AWOTFGameModeBase::AWOTFGameModeBase()
{
	GameStateClass = AWOTFGameState::StaticClass();
	PlayerStateClass = AWOTFPlayerState::StaticClass();
	HUDClass = AWOTFHudBase::StaticClass();
	PlayerControllerClass = AWOTFPlayerControllerBase::StaticClass();
}

void AWOTFGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	FLogUtil::Log(TEXT("GameMode Base"));
}


