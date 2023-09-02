#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FItemWeapon.generated.h"

USTRUCT(BlueprintType)
struct FItemWeapon : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float FireRate;
};
