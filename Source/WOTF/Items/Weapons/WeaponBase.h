// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ItemBase.h"
#include "FItemWeapon.h"
#include "WeaponBase.generated.h"

class UItemWeaponDataTable;
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
protected:
	virtual void InvalidateItemState() const override;
private:
	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties", meta=(AllowPrivateAccess="true"))
	FItemWeapon WeaponData;
	void PlayFireAnimation() const;
public:
	virtual void Fire(const FVector& Vector);
	FItemWeapon& GetWeaponData()
	{
		return WeaponData;
	}
};
