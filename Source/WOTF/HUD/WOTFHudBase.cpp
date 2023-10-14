// Fill out your copyright notice in the Description page of Project Settings.


#include "WOTFHudBase.h"


// Sets default values
AWOTFHudBase::AWOTFHudBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AWOTFHudBase::DrawHUD()
{
	Super::DrawHUD();
}

// Called when the game starts or when spawned
void AWOTFHudBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWOTFHudBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

