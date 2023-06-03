// Copyright Epic Games, Inc. All Rights Reserved.


#include "WOTFGameModeBase.h"
#include "utils/FLogUtil.h"


AWOTFGameModeBase::AWOTFGameModeBase(){}

void AWOTFGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	FLogUtil::Log(TEXT("GameMode Base"));
}


