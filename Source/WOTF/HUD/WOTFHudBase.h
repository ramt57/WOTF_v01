// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WOTFHudBase.generated.h"

UCLASS()
class WOTF_API AWOTFHudBase : public AHUD
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWOTFHudBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
