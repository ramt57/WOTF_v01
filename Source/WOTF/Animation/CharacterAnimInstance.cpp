// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	if(Character == nullptr) return;
	Velocity = Character->GetCharacterMovement()->Velocity;
	GroundSpeed = Velocity.Size2D();
	IsFalling = Character->GetCharacterMovement()->IsFalling();
	ShouldMove = Character->GetCharacterMovement()->IsMovingOnGround() && GroundSpeed > 3.0f;
	IsPrimaryWeaponEquip = Character->GetActionCombat()->IsPrimaryWeaponEquipped();
	IsSecondaryWeaponEquip = Character->GetActionCombat()->IsSecondaryWeaponEquipped();
	IsMeleeWeaponEquip = Character->GetActionCombat()->IsMeleeWeaponEquipped();
}
