// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FItemWeapon.h"
#include "ItemWeaponDataTable.generated.h"

/**
 * 
 */
UCLASS()
class WOTF_API UItemWeaponDataTable : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Weapon Data")
	static bool GetWeaponData(FName WeaponName, FItemWeapon& OutWeaponData);
};
