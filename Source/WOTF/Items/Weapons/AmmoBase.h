// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ItemBase.h"
#include "AmmoBase.generated.h"

UCLASS()
class WOTF_API AAmmoBase : public AItemBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category="Ammo Properties")
	int32 AmmoCount;
	
public:
	// Sets default values for this actor's properties
	AAmmoBase();
	FORCEINLINE int32 GetAmmoCount() const
	{
		return AmmoCount;
	}
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void InvalidateItemState() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
