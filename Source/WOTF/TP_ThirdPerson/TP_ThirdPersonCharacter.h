// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterInterface.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TP_ThirdPersonCharacter.generated.h"


UCLASS(config=Game)
class ATP_ThirdPersonCharacter : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(Replicated)
	class AWeaponBase* PrimaryWeapon;
	UPROPERTY(Replicated)
	AWeaponBase* SecondaryWeapon;
	UPROPERTY(Replicated)
	AWeaponBase* MeleeWeapon;
	UPROPERTY(Replicated)
	TArray<AWeaponBase*> ThrowableWeapons;

public:
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

private:
	/* Line Trace Under Crosshairs*/
	bool TraceUnderCrosshairs(FHitResult& OutHitResult);

	UPROPERTY(ReplicatedUsing = OnRep_OverlappedItemBase)
	class AItemBase* OverlappedItemBase;
	UFUNCTION()
	void OnRep_OverlappedItemBase(AItemBase* PrevValue) const;
	UPROPERTY()
	AActor* LastHitItemBase;
	void StartLineTraceForItems();
	virtual void SetOverlappedItemBase_Implementation(AItemBase* ItemBase) override;

public:
	ATP_ThirdPersonCharacter();
	virtual void Tick(float DeltaSeconds) override;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
