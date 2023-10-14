// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWeaponDataTable.h"

bool UItemWeaponDataTable::GetWeaponData(const FName WeaponName, FItemWeapon& OutWeaponData)
{
	// Replace with the correct asset reference path
	const UDataTable* WeaponDataTable = LoadObject<UDataTable>(
		nullptr, TEXT("DataTable'/Game/Game_/Weapon/WeaponDataTable.WeaponDataTable'"));
	
	if (!WeaponDataTable)
	{
		// Handle error, data table not found
		return false;
	}

	if (const FItemWeapon* WeaponData = WeaponDataTable->FindRow<FItemWeapon>(WeaponName, TEXT("")))
	{
		OutWeaponData = *WeaponData; // Copy the data to the OutWeaponData parameter
		return true;
	}
	// Handle error, weapon data not found
	return false;
}
