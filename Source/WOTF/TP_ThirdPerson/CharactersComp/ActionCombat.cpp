// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionCombat.h"

#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "WOTF/Items/EItemState.h"
#include "WOTF/Items/Weapons/EWeaponType.h"
#include "WOTF/Items/Weapons/WeaponBase.h"
#include "WOTF/utils/FLogUtil.h"

// Sets default values for this component's properties
UActionCombat::UActionCombat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActionCombat::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UActionCombat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UActionCombat, EquippedWeapon);
}

// Called every frame
void UActionCombat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UActionCombat::ServerEquipWeapon_Implementation(ACharacter* Character, AWeaponBase* Weapon)
{
	EquipWeapon(Character, Weapon);
}

void UActionCombat::EquipWeapon(ACharacter* Character, AWeaponBase* Weapon)
{
	switch (Weapon->WeaponType)
	{
	case EWeaponType::Melee: break;
	case EWeaponType::Primary: break;
	case EWeaponType::Secondary:
		{
			EquippedWeapon = Weapon;
			EquippedWeapon->SetItemState(EItemState::Equipped);
			if (const USkeletalMeshSocket* MeshSocket = Character->GetMesh()->GetSocketByName(
				FName("hand_rSocket")))
			{
				MeshSocket->AttachActor(Weapon, Character->GetMesh());
			}
			EquippedWeapon->SetOwner(Character);
			OnEquipWeapon.Broadcast(Character, Weapon);
			break;
		}
	case EWeaponType::Throwable: break;
	default: ;
	}
}

void UActionCombat::PickupItem(ACharacter* Character, AItemBase* Item)
{
	if (Item)
	{
		switch (Item->GetItemType())
		{
		case EItemType::Weapon:
			{
				if (GetOwner()->HasAuthority())
				{
					EquipWeapon(Character, Cast<AWeaponBase>(Item));
				}
				else
				{
					ServerEquipWeapon(Character, Cast<AWeaponBase>(Item));
				}

				break;
			}
		case EItemType::Aid: break;
		case EItemType::Key: break;
		case EItemType::Misc: break;
		default: ;
		}
	}
}
