// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "WOTF/DataTables/ItemWeaponDataTable.h"
#include "WOTF/Items/EItemState.h"

AWeaponBase::AWeaponBase()
{
	SetItemType(EItemType::Weapon);
}

void AWeaponBase::InvalidateItemState() const
{
	Super::InvalidateItemState();
	switch (GetItemState())
	{
	case EItemState::Initial:
		{
			GetStaticMeshComponent()->SetVisibility(true);
			GetSkeletonMeshComponent()->SetVisibility(false);
			break;
		}
	case EItemState::Pickup: break;
	case EItemState::Dropped:
		{
			GetStaticMeshComponent()->SetVisibility(true);
			GetSkeletonMeshComponent()->SetVisibility(false);
			break;
		}
	case EItemState::AddedToInventory: break;
	case EItemState::Equipped:
		{
			GetStaticMeshComponent()->SetVisibility(false);
			GetSkeletonMeshComponent()->SetVisibility(true);
			break;
		}
	case EItemState::MaxCount: break;
	default: ;
	}
}

void AWeaponBase::Fire(const FVector& Vector)
{
	PlayFireAnimation();
}

void AWeaponBase::PlayFireAnimation() const
{
	GetSkeletonMeshComponent()->PlayAnimation(WeaponData.FireAnimAssets, false);
}
