// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ItemBase.h"
#include "WeaponBase.generated.h"

enum class EWeaponType : uint8;
/**
 * 
 */
UCLASS()
class WOTF_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()
public:
	AWeaponBase();
	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType;
};