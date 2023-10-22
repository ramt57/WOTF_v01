// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionCombat.h"

#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "WOTF/Items/EItemState.h"
#include "WOTF/Items/Weapons/AmmoBase.h"
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
	Character = Cast<ACharacter>(GetOwner());
	CharacterAnimInstance = Character->GetMesh()->GetAnimInstance();
	DefaultMeshRotator = Character->GetMesh()->GetRelativeRotation();
	BaseMaxWalkSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
	BaseMaxWalkCrouchSpeed = Character->GetCharacterMovement()->MaxWalkSpeedCrouched;
}

void UActionCombat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UActionCombat, EquippedWeapon);
	DOREPLIFETIME(UActionCombat, IsAiming);
	DOREPLIFETIME(UActionCombat, DefaultMeshRotator);
	DOREPLIFETIME(UActionCombat, bCanFire);
	DOREPLIFETIME_CONDITION(UActionCombat, CarriedAmmo, COND_OwnerOnly);
}

void UActionCombat::MultiCast_Fire_Implementation(const FVector_NetQuantize& HitTarget)
{
	if (EquippedWeapon != nullptr)
	{
		if (CharacterAnimInstance && FireWeaponAnimMontage)
		{
			// FLogUtil::PrintString("Weapon Fired");
			EquippedWeapon->Fire(HitTarget);
			CharacterAnimInstance->Montage_Play(FireWeaponAnimMontage);
			const FName SectionName = FName("Stand_Fire_Single");
			CharacterAnimInstance->Montage_JumpToSection(SectionName);
		}
	}
}

bool UActionCombat::CanFire() const
{
	if (!EquippedWeapon || EquippedWeapon->IsAmmoEmpty())
	{
		FLogUtil::PrintString("No Ammo. Please Reload");
		return false;
	}
	return bCanFire;
}

void UActionCombat::OnRep_CarriedAmmo()
{
}

bool UActionCombat::TraceUnderCrosshairs(FHitResult& OutHitResult)
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
		const FVector End = {Start + CrosshairWorldDirection * 50000.f};
		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECC_Visibility);
		if (!OutHitResult.bBlockingHit)
		{
			OutHitResult.ImpactPoint = End;
			return false;
		}
		return true;
	}
	return false;
}

void UActionCombat::StartFireTimer()
{
	if (EquippedWeapon == nullptr) return;
	// Start the timer if the player can't fire immediately
	if (!Character->GetWorldTimerManager().IsTimerActive(FireTimer))
	{
		Character->GetWorldTimerManager().SetTimer(FireTimer, this, &UActionCombat::FireTimerCallback,
		                                           EquippedWeapon->GetWeaponData().FireRate, true, 0.f);
	}
}

void UActionCombat::FireTimerCallback()
{
	FireWeapon();
}

void UActionCombat::EnableFiring()
{
	bCanFire = true;
}

void UActionCombat::FireWeapon()
{
	if (!CanFire()) return;
	if (Character->IsLocallyControlled())
	{
		// Prevent firing until the next shot can be taken
		bCanFire = false;
		Character->GetWorldTimerManager().SetTimer(FireCooldownTimer, this, &UActionCombat::EnableFiring,
		                                           EquippedWeapon->GetWeaponData().FireRate, false);
		FHitResult OutHitResult;
		TraceUnderCrosshairs(OutHitResult);
		ServerFire(OutHitResult.ImpactPoint);
	}
}

void UActionCombat::FireButtonPressed(const bool IsPressed)
{
	if (IsPressed)
	{
		if (EquippedWeapon->GetWeaponData().bIsAutomaticWeapon)
		{
			StartFireTimer();
		}
		else
		{
			FireWeapon();
		}
	}
	else
	{
		Character->GetWorldTimerManager().ClearTimer(FireTimer);
	}
}

void UActionCombat::ReloadWeapon()
{
	FLogUtil::PrintString("Reloading...");
	if (EquippedWeapon == nullptr) return;
	const TSubclassOf<AAmmoBase> Ammo = EquippedWeapon->GetWeaponData().Ammo->GetClass();
	if (const int8 AmmoCount = WithdrawAmmo(AAmmoBase::StaticClass(), EquippedWeapon->GetWeaponData().ClipSize); AmmoCount > 0)
	{
		AddAmmo(AAmmoBase::StaticClass(), EquippedWeapon->GetCurrentAmmo());
		EquippedWeapon->SetCurrentAmmo(AmmoCount);
	}
	else
	{
		FLogUtil::PrintString("No Ammo Found");
	}
}

void UActionCombat::ServerFire_Implementation(const FVector_NetQuantize& HitTarget)
{
	MultiCast_Fire(HitTarget);
}

// Called every frame
void UActionCombat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FHitResult HitResult;
	TraceUnderCrosshairs(HitResult);
	HitImpactPoint = HitResult.ImpactPoint;
	DebugLineThroughMuzzle();
}

void UActionCombat::ServerEquipWeapon_Implementation(AWeaponBase* Weapon)
{
	EquipWeapon(Weapon);
}

void UActionCombat::OnRep_IsAiming() const
{
	if (EquippedWeapon)
	{
		Character->bUseControllerRotationYaw = IsAiming ? true : false;
		Character->GetCharacterMovement()->MaxWalkSpeedCrouched = IsAiming
			                                                          ? AimMaxWalkCrouchSpeed
			                                                          : BaseMaxWalkCrouchSpeed;
		Character->GetCharacterMovement()->MaxWalkSpeed = IsAiming ? AimMaxWalkSpeed : BaseMaxWalkSpeed;
	}
}

void UActionCombat::CheckAndSetAiming(const bool bIsAiming)
{
	if (EquippedWeapon)
	{
		IsAiming = bIsAiming;
		Character->bUseControllerRotationYaw = IsAiming ? true : false;
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

void UActionCombat::DebugLineThroughMuzzle() const
{
	if (EquippedWeapon)
	{
		FTransform MuzzleTipTransform = EquippedWeapon->GetSkeletonMeshComponent()->GetSocketTransform(
			EquippedWeapon->GetWeaponData().ProjectileSpawnSocket, RTS_World);
		FVector MuzzleX(FRotationMatrix(MuzzleTipTransform.GetRotation().Rotator()).GetUnitAxis(EAxis::X));
		DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), MuzzleTipTransform.GetLocation() + MuzzleX * 1000.f,
		              FColor::Red);
		DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), HitImpactPoint,
		              FColor::Green);
	}
}

void UActionCombat::AddAmmo(const TSubclassOf<AAmmoBase> AmmoType, const int32 ClipSize)
{
	if (int32* ExistingAmmo = AmmoInventory.Find(AmmoType))
	{
		// Add a full magazine to the existing count
		*ExistingAmmo += ClipSize;
	}
	else
	{
		// Add a new entry for this type of ammo with a full magazine
		AmmoInventory.Add(AmmoType, ClipSize);
	}
}

int32 UActionCombat::WithdrawAmmo(const TSubclassOf<AAmmoBase> AmmoType, const int32 ClipSize)
{
	if (int32* ExistingAmmo = AmmoInventory.Find(AmmoType))
	{
		// Check if the amount is less than a full magazine size
		if (*ExistingAmmo < ClipSize)
		{
			// Fully remove the ammo clip from inventory and store the amount withdrawn
			const int32 WithdrawnAmmo = *ExistingAmmo;
			AmmoInventory.Remove(AmmoType);
			return WithdrawnAmmo;
		}
		// Subtract a full magazine from the existing count and return a full magazine
		*ExistingAmmo -= ClipSize;
		return ClipSize;
	}

	// If the ammo type wasn't found in the inventory, return 0 to indicate nothing was withdrawn.
	return 0;
}

void UActionCombat::EquipAmmo(AAmmoBase* Ammo)
{
	Ammo->SetItemState(EItemState::Picked);
	Ammo->SetOwner(Character);
	OnEquipAmmo.Broadcast(Ammo);
}

void UActionCombat::EquipWeapon(AWeaponBase* Weapon)
{
	EquippedWeapon = Weapon;
	EquippedWeapon->SetItemState(EItemState::Equipped);
	EquippedWeapon->SetOwner(Character);
	OnEquipWeapon.Broadcast(Weapon);
	switch (Weapon->GetWeaponData().WeaponType)
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


void UActionCombat::PickupItem(AItemBase* Item)
{
	if (Item)
	{
		switch (Item->GetItemType())
		{
		case EItemType::Weapon:
			{
				if (GetOwner()->HasAuthority())
				{
					EquipWeapon(Cast<AWeaponBase>(Item));
				}
				else
				{
					ServerEquipWeapon(Cast<AWeaponBase>(Item));
				}
				break;
			}
		case EItemType::Aid: break;
		case EItemType::Key: break;
		case EItemType::Misc: break;
		case EItemType::Ammo:
			{
				EquipAmmo(Cast<AAmmoBase>(Item));
				break;
			}
		default: ;
		}
	}
}
