// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WOTF_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(Replicated)
	class AWeaponBase* PrimaryWeapon;
	UPROPERTY(Replicated)
	AWeaponBase* SecondaryWeapon;
	UPROPERTY(Replicated)
	AWeaponBase* MeleeWeapon;
	UPROPERTY(Replicated)
	TArray<AWeaponBase*> ThrowableWeapons;
public:
	UCombatComponent();
	FORCEINLINE void SetPrimaryWeapon(AWeaponBase* Weapon)
	{
		this->PrimaryWeapon = Weapon;
	}

	FORCEINLINE void SetSecondaryWeapon(AWeaponBase* Weapon)
	{
		this->SecondaryWeapon = Weapon;
	}

	FORCEINLINE void SetMeleeWeapon(AWeaponBase* Weapon)
	{
		this->MeleeWeapon = Weapon;
	}

	FORCEINLINE void SetThrowableWeapons(const TArray<AWeaponBase*>& Weapons)
	{
		this->ThrowableWeapons = Weapons;
	}
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
