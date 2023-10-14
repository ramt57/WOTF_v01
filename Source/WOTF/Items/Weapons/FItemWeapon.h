#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FItemWeapon.generated.h"

enum class EWeaponType : uint8;
class AProjectile;

USTRUCT(BlueprintType)
struct FItemWeapon : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FName WeaponName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Weapon")
	EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FName AmmoId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int ClipSize;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float FireRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool isProjectileWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AProjectile> Projectile;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FName ProjectileSpawnSocket;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	UAnimationAsset* FireAnimAssets;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	UAnimMontage* CharacterFireAnimation;
};
