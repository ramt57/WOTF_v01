// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "WOTF/DataTables/ItemWeaponDataTable.h"
#include "WOTF/Items/EItemState.h"
#include "WOTF/utils/FLogUtil.h"

AWeaponBase::AWeaponBase()
{
	SetItemType(EItemType::Weapon);
}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponBase, CurrentAmmo);
}

void AWeaponBase::InvalidateItemState()
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
	case EItemState::Picked: break;
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
	SpendRound();
}

void AWeaponBase::PlayFireAnimation() const
{
	GetSkeletonMeshComponent()->PlayAnimation(WeaponData.FireAnimAssets, false);
}

void AWeaponBase::OnRep_CurrentAmmo()
{
}

void AWeaponBase::SpendRound()
{
	CurrentAmmo = FMath::Clamp(CurrentAmmo -1, 0, WeaponData.ClipSize);
	FLogUtil::PrintString(FString::Printf(TEXT("CurrentAmmo: %d"), CurrentAmmo));
}
