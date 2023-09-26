// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "WOTF/Items/EItemState.h"
#include "WOTF/TP_ThirdPerson/TP_ThirdPersonCharacter.h"

AWeaponBase::AWeaponBase()
{
	SetItemType(EItemType::Weapon);
}

void AWeaponBase::Fire(const FVector& Vector)
{
	PlayFireAnimation();
}

void AWeaponBase::PlayFireAnimation() const
{
	GetSkeletalMesh()->PlayAnimation(FireAnimAssets, false);
}
