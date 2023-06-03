// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WOTF/WOTFGameModeBase.h"
#include "L1_GameMode.generated.h"

UCLASS()
class WOTF_API AL1_GameMode : public AWOTFGameModeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AL1_GameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
