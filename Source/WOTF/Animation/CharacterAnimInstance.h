// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WOTF_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Refrences", meta = (AllowPrivateAccess = "true"))
	class ATP_ThirdPersonCharacter* Character;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	bool ShouldMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	bool IsCrouched;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat Movement")
	bool IsAiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Equip Data")
	bool IsPrimaryWeaponEquip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Equip Data")
	bool IsSecondaryWeaponEquip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Equip Data")
	bool IsMeleeWeaponEquip;
};
