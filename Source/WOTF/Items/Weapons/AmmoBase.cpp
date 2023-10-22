// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBase.h"

#include "WOTF/Items/EItemState.h"


// Sets default values
AAmmoBase::AAmmoBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAmmoBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAmmoBase::InvalidateItemState()
{
	Super::InvalidateItemState();
	switch (GetItemState())
	{
	case EItemState::Picked:
		{
			Destroy();
			break;
		}
	default: break;;
	}
}

// Called every frame
void AAmmoBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

