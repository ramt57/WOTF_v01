// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionCombat.h"

#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "WOTF/Items/EItemState.h"
#include "WOTF/Items/Weapons/EWeaponType.h"
#include "WOTF/Items/Weapons/WeaponBase.h"
#include "WOTF/utils/FLogUtil.h"

// Sets default values for this component's properties
UActionCombat::UActionCombat()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UActionCombat::BeginPlay()
{
	Super::BeginPlay();
	if (const auto Character = Cast<ACharacter>(GetOwner()))
	{
		BaseMaxWalkSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
		BaseMaxWalkCrouchSpeed = Character->GetCharacterMovement()->MaxWalkSpeedCrouched;
	}
}

void UActionCombat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UActionCombat, EquippedWeapon);
	DOREPLIFETIME(UActionCombat, IsAiming);
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

void UActionCombat::CheckAndSetAiming(const bool bIsAiming)
{
	if (EquippedWeapon)
	{
		IsAiming = bIsAiming;
		const auto Character = Cast<ACharacter>(GetOwner());
		Character->GetCharacterMovement()->MaxWalkSpeedCrouched = IsAiming
			                                                          ? AimMaxWalkCrouchSpeed
			                                                          : BaseMaxWalkCrouchSpeed;
		Character->GetCharacterMovement()->MaxWalkSpeed = IsAiming ? AimMaxWalkSpeed : BaseMaxWalkSpeed;
	}
}

void UActionCombat::SetAiming(const bool bIsAiming)
{
	if (GetOwner()->HasAuthority())
	{
		CheckAndSetAiming(bIsAiming);
	}
	else
	{
		ServerSetAiming(bIsAiming);
	}
}

void UActionCombat::ServerSetAiming_Implementation(const bool bIsAiming)
{
	CheckAndSetAiming(bIsAiming);
}

void UActionCombat::EquipWeapon(ACharacter* Character, AWeaponBase* Weapon)
{
	EquippedWeapon = Weapon;
	EquippedWeapon->SetItemState(EItemState::Equipped);
	EquippedWeapon->SetOwner(Character);
	OnEquipWeapon.Broadcast(Character, Weapon);
	switch (Weapon->WeaponType)
	{
	case EWeaponType::Melee: break;
	case EWeaponType::Primary:
		{
			if (const USkeletalMeshSocket* MeshSocket = Character->GetMesh()->GetSocketByName(
				FName("hand_r_pistol")))
			{
				MeshSocket->AttachActor(Weapon, Character->GetMesh());
			}
			break;
		}
	case EWeaponType::Secondary:
		{
			if (const USkeletalMeshSocket* MeshSocket = Character->GetMesh()->GetSocketByName(
				FName("hand_r_rifle")))
			{
				MeshSocket->AttachActor(Weapon, Character->GetMesh());
			}
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
