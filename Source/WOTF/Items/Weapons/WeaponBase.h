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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void InvalidateItemState() override;
private:
	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties", meta=(AllowPrivateAccess="true"))
	FItemWeapon WeaponData;
	void PlayFireAnimation() const;
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_CurrentAmmo, Category="Weapon Properties")
	int32 CurrentAmmo;

public:
	void SetCurrentAmmo(const int8 AmmoCount)
	{
		this->CurrentAmmo = AmmoCount;
	}
	FORCEINLINE int32 GetCurrentAmmo()
	{
		return CurrentAmmo;
	}
private:
	UFUNCTION()
	void OnRep_CurrentAmmo();
	void SpendRound();
public:
	FORCEINLINE bool IsAmmoEmpty() const
	{
		return CurrentAmmo <= 0;
	}
	virtual void Fire(const FVector& Vector);
	FItemWeapon& GetWeaponData()
	{
		return WeaponData;
	}
};
