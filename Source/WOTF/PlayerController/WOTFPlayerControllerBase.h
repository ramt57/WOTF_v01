﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WOTFPlayerControllerBase.generated.h"

UCLASS()
class WOTF_API AWOTFPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWOTFPlayerControllerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
