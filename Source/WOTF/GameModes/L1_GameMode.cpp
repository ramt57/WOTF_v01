// Fill out your copyright notice in the Description page of Project Settings.


#include "L1_GameMode.h"
// Sets default values
AL1_GameMode::AL1_GameMode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AL1_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AL1_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

