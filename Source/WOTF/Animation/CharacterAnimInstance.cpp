// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WOTF/TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "WOTF/TP_ThirdPerson/CharactersComp/ActionCombat.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<ATP_ThirdPersonCharacter>(TryGetPawnOwner());
}


void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Character == nullptr) return;
	TurnInPlace = Character->GetTurnInPlace();
	Velocity = Character->GetCharacterMovement()->Velocity;
	GroundSpeed = Velocity.Size2D();
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Character->GetActorRotation());
	IsFalling = Character->GetCharacterMovement()->IsFalling();
	ShouldMove = Character->GetCharacterMovement()->IsMovingOnGround() && GroundSpeed > 3.0f;
	IsPrimaryWeaponEquip = Character->GetActionCombat()->IsPrimaryWeaponEquipped();
	IsSecondaryWeaponEquip = Character->GetActionCombat()->IsSecondaryWeaponEquipped();
	IsMeleeWeaponEquip = Character->GetActionCombat()->IsMeleeWeaponEquipped();
	IsCrouched = Character->bIsCrouched;
	IsAiming = Character->GetActionCombat()->GetIsAiming();
	AO_Yaw = Character->GetAO_Yaw();
	AO_Pitch = Character->GetAO_Pitch();
	IsSprinting = Character->GetCharacterMovement()->IsMovingOnGround() && GroundSpeed > 350.0f;
	if (Character->IsLocallyControlled())
	{
		CalculateLookingPosition();
	}
}

void UCharacterAnimInstance::CalculateLookingPosition()
{
	/* Get ViewPort Size */
	FVector2D ViewPortSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
	}

	/* Get Screen space location of cross hairs which is usually a center of screen for most of the games*/
	const FVector2D CrosshairLocation(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	/* Get World position and direction of cross hairs */
	if (UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
	                                             CrosshairLocation,
	                                             CrosshairWorldPosition,
	                                             CrosshairWorldDirection))
	{
		/* Trace from cross hairs world location outward */
		const FVector Start = {CrosshairWorldPosition};
		CharacterLookingWorldPosition = {Start + CrosshairWorldDirection * 50000.f};
	}
}
