// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WOTF/Items/Weapons/EWeaponType.h"
#include "WOTF/Items/Weapons/WeaponBase.h"
#include "ActionCombat.generated.h"
class AItemBase;
class AWeaponBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquippedWeapon, ACharacter*, Character, AWeaponBase*, WeaponB);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WOTF_API UActionCombat : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UActionCombat();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(Replicated, VisibleAnywhere)
	AWeaponBase* EquippedWeapon;
	UPROPERTY(ReplicatedUsing=OnRep_IsAiming, VisibleAnywhere)
	bool IsAiming;
	void ToggleControllerRotationYawOnAiming() const;
	UFUNCTION()
	void OnRep_IsAiming() const;
	UPROPERTY(Replicated)
	FRotator DefaultMeshRotator;
public:
	void CheckAndSetAiming(bool bIsAiming);
	void SetAiming(const bool bIsAiming);
	UFUNCTION(Server, Unreliable)
	void ServerSetAiming(const bool bIsAiming);

private:
	float BaseMaxWalkSpeed;
	float BaseMaxWalkCrouchSpeed;
	UPROPERTY(EditAnywhere)
	float AimMaxWalkSpeed = 300.f;
	UPROPERTY(EditAnywhere)
	float AimMaxWalkCrouchSpeed = 100.f;

	UFUNCTION(Server, Reliable)
	void ServerEquipWeapon(ACharacter* Character, AWeaponBase* Weapon);
	void EquipWeapon(ACharacter* Character, AWeaponBase* Weapon);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Equip Weapon")
	FOnEquippedWeapon OnEquipWeapon;

	UFUNCTION()
	void PickupItem(ACharacter* Character, AItemBase* Weapon);
	FORCEINLINE bool IsPrimaryWeaponEquipped() const
	{
		return EquippedWeapon && EquippedWeapon->WeaponType == EWeaponType::Primary;
	}

	FORCEINLINE bool IsSecondaryWeaponEquipped() const
	{
		return EquippedWeapon && EquippedWeapon->WeaponType == EWeaponType::Secondary;
	}

	FORCEINLINE bool IsMeleeWeaponEquipped() const
	{
		return EquippedWeapon && EquippedWeapon->WeaponType == EWeaponType::Melee;
	}

	FORCEINLINE bool GetIsAiming() const
	{
		return IsAiming;
	}
	FORCEINLINE AWeaponBase* GetEquipWeapon() const
	{
		return EquippedWeapon;
	}
};
