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

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	FVector HitImpactPoint;
	UPROPERTY(Replicated, VisibleAnywhere)
	AWeaponBase* EquippedWeapon;
	UPROPERTY(ReplicatedUsing=OnRep_IsAiming, VisibleAnywhere)
	bool IsAiming;
	void ToggleControllerRotationYawOnAiming() const;
	UFUNCTION()
	void OnRep_IsAiming() const;
	UPROPERTY(Replicated)
	FRotator DefaultMeshRotator;
	UFUNCTION(Server, Unreliable)
	void ServerSetAiming(const bool bIsAiming);
	void CheckAndSetAiming(bool bIsAiming);

	float BaseMaxWalkSpeed;
	float BaseMaxWalkCrouchSpeed;
	UPROPERTY(EditAnywhere)
	float AimMaxWalkSpeed = 300.f;
	UPROPERTY(EditAnywhere)
	float AimMaxWalkCrouchSpeed = 100.f;

	UFUNCTION(Server, Reliable)
	void ServerEquipWeapon(ACharacter* Character, AWeaponBase* Weapon);
	void DebugLineThroughMuzzle();
	void EquipWeapon(ACharacter* Character, AWeaponBase* Weapon);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY()
	UAnimInstance* CharacterAnimInstance;
	UPROPERTY(EditAnywhere, Category=Combat)
	UAnimMontage* FireWeaponAnimMontage;
	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& HitTarget);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_Fire(const FVector_NetQuantize& HitTarget);
	bool IsLocallyControlled = false;
public:
	/* Line Trace Under Crosshairs*/
	bool TraceUnderCrosshairs(FHitResult& OutHitResult);
	void FireButtonPressed(bool IsPressed);
	void SetAiming(const bool bIsAiming);
	UPROPERTY(BlueprintAssignable, Category = "Equip Weapon")
	FOnEquippedWeapon OnEquipWeapon;

	UFUNCTION()
	void PickupItem(ACharacter* Character, AItemBase* Weapon);
	FORCEINLINE bool IsPrimaryWeaponEquipped() const
	{
		return EquippedWeapon && EquippedWeapon->GetWeaponData().WeaponType == EWeaponType::Primary;
	}

	FORCEINLINE bool IsSecondaryWeaponEquipped() const
	{
		return EquippedWeapon && EquippedWeapon->GetWeaponData().WeaponType == EWeaponType::Secondary;
	}

	FORCEINLINE bool IsMeleeWeaponEquipped() const
	{
		return EquippedWeapon && EquippedWeapon->GetWeaponData().WeaponType == EWeaponType::Melee;
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
