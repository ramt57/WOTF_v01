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

	/** Pick Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PickAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Aiming Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimingAction;

	/** Firing Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FiringAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;
	
	UPROPERTY(ReplicatedUsing = OnRep_OverlappedItemBase)
	class AItemBase* OverlappedItemBase;
	UFUNCTION()
	void OnRep_OverlappedItemBase(AItemBase* PrevValue) const;
	UPROPERTY()
	AActor* LastHitItemBase;
	void StartLineTraceForItems();
	virtual void SetOverlappedItemBase_Implementation(AItemBase* ItemBase) override;

	UPROPERTY(Category="Action Combat", VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UActionCombat* ActionCombat;

public:
	FORCEINLINE float& GetAO_Yaw()
	{
		return AO_Yaw;
	}

	FORCEINLINE float& GetAO_Pitch()
	{
		return AO_Pitch;
	}

private:
	UPROPERTY()
	float AO_Yaw;

	UPROPERTY()
	float AO_Pitch;
	void CalculateAimOffset(float DeltaSeconds);
	UPROPERTY()
	UAnimInstance* AnimInstance;
public:
	float DefaultCameraFov;
	float CurrentCameraFov;
	void InterpFOV(float DeltaTime);
	FORCEINLINE UActionCombat*& GetActionCombat()
	{
		return ActionCombat;
	}

	ATP_ThirdPersonCharacter();
	virtual void Tick(float DeltaSeconds) override;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for pick input */
	void Pick();
	void CrouchPressed();
	void AimingPressed();
	void FirePressed();
	void FireReleased();
	void ReloadWeapon();
	UFUNCTION()
	void EquippedWeapon(class AWeaponBase* Weapon);
	UFUNCTION()
	void EquipAmmo(class AAmmoBase* AmmoBase);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	
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
