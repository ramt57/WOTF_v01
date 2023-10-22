#pragma once

UENUM(BlueprintType)
enum class EItemState : uint8
{
	Initial UMETA(DisplayName = "Intial State"),
	Picked UMETA(DisplayName = "Picked"),
	Dropped UMETA(DisplayName = "Dropped"),
	AddedToInventory UMETA(DisplayName = "Added to Inventory"),
	Equipped UMETA(DisplayName = "Equipped"),
	MaxCount UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class  EItemType: uint8
{
	Aid UMETA(DisplayName ="Aid"),
	Weapon UMETA(DisplayName ="Weapon"),
	Ammo UMETA(DisplayName ="Ammo"),
	Key UMETA(DisplayName ="Key"),
	Misc UMETA(DisplayName ="Misc"),
};
