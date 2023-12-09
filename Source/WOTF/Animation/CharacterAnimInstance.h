// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WOTF/TP_ThirdPerson/ECharacterEnums.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Turn", meta=(AllowPrivateAccess="true"))
	ETurnInPlace TurnInPlace;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Look", meta=(AllowPrivateAccess="true"))
	FVector CharacterLookingWorldPosition = FVector(0);
	void CalculateLookingPosition();
public:
	FORCEINLINE ATP_ThirdPersonCharacter* GetCharacter() const
	{
		return Character;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	bool ShouldMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	bool IsSprinting;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim Offset")
	float AO_Yaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim Offset")
	float AO_Pitch;
};
