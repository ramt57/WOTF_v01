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
	DOREPLIFETIME(UActionCombat, PrimaryWeapon);
	DOREPLIFETIME(UActionCombat, MeleeWeapon);
	DOREPLIFETIME(UActionCombat, SecondaryWeapon);
	DOREPLIFETIME(UActionCombat, ThrowableWeapons);
}

// Called every frame
void UActionCombat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UActionCombat::EquippedWeapon(ACharacter* Character, AWeaponBase* Weapon)
{
	if (Weapon)
	{
		switch (Weapon->WeaponType)
		{
		case EWeaponType::Melee:
			{
			}
		case EWeaponType::Primary:
			{
			}
		case EWeaponType::Secondary:
			{
				SecondaryWeapon = Weapon;
				SecondaryWeapon->ItemState = EItemState::Equipped;
				if(const USkeletalMeshSocket* MeshSocket = Character->GetMesh()->GetSocketByName(FName("hand_rSocket")))
				{
					MeshSocket->AttachActor(Weapon, Character->GetMesh());
				}
				SecondaryWeapon->SetOwner(Character);
				if (SecondaryWeapon->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
				{
					SecondaryWeapon->Execute_ToggleVisibilityOfItemPickupWidget(SecondaryWeapon, false);
				}
				OnEquipWeapon.Broadcast(Character, Weapon);
			}
		case EWeaponType::Throwable:
			{
			}
		default:
			{
			}
		}
	}
}
