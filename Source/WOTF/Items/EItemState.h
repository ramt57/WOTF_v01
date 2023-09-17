#pragma once

UENUM(BlueprintType)
enum class EItemState : uint8
{
	Initial UMETA(DisplayName = "Intial State"),
	Pickup UMETA(DisplayName = "Pickup"),
	Dropped UMETA(DisplayName = "Dropped"),
	AddedToInventory UMETA(DisplayName = "Added to Inventory"),
	Equipped UMETA(DisplayName = "Equipped"),
	MaxCount UMETA(DisplayName = "DefaultMAX")
};
