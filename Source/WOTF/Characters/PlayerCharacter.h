// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
class UInputComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class WOTF_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	/* Camera Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Camera, meta=(AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/* Input Mapping Context*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
	UInputMappingContext* DefaultMappingContext;

	/* Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/* Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
public:
	
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
public:
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	/** Returns FirstPersonCameraComponent subObject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};
