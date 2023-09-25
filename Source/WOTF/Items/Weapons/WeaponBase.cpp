// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "WOTF/TP_ThirdPerson/TP_ThirdPersonCharacter.h"

AWeaponBase::AWeaponBase()
{

}

void AWeaponBase::PlayFireAnimation() const
{
	GetSkeletalMesh()->PlayAnimation(FireAnimAssets, false);
}
