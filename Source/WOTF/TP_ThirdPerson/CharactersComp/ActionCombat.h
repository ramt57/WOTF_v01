// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionCombat.generated.h"
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
	class ATP_ThirdPersonCharacter* ThirdPersonCharacter;
	UPROPERTY(Replicated, VisibleAnywhere)
	AWeaponBase* PrimaryWeapon;
	UPROPERTY(Replicated, VisibleAnywhere)
	AWeaponBase* SecondaryWeapon;
	UPROPERTY(Replicated, VisibleAnywhere)
	AWeaponBase* MeleeWeapon;
	UPROPERTY(Replicated, VisibleAnywhere)
	TArray<AWeaponBase*> ThrowableWeapons;
	
	UPROPERTY(BlueprintAssignable, Category = "Equip Weapon")
	FOnEquippedWeapon OnEquipWeapon;
public:
	UFUNCTION()
	void EquippedWeapon(ACharacter* Character,AWeaponBase* Weapon);
	friend class ATP_ThirdPersonCharacter;
};
