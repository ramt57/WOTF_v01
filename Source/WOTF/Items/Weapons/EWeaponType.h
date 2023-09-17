#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Melee UMETA(DisplayName = "Melee Weapon"),
	Primary UMETA(DisplayName = "Primary Weapon"),
	Secondary UMETA(DisplayName = "Secondary Weapon"),
	Throwable UMETA(DisplayName = "Throwable Weapon"),
};
