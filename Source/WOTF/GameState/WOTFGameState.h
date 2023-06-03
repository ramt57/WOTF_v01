// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WOTFGameState.generated.h"

UCLASS()
class WOTF_API AWOTFGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWOTFGameState();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
