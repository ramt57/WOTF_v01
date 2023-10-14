// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"

#include "Projectile.h"
#include "Engine/SkeletalMeshSocket.h"


// Sets default values
AProjectileWeapon::AProjectileWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectileWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileWeapon::Fire(const FVector& HitVector)
{
	Super::Fire(HitVector);
	if (!HasAuthority()) return;
	APawn* Pawn = Cast<APawn>(GetOwner()); //TODO("Can be removed from here and optimised")
	if (const USkeletalMeshSocket* SkeletalMeshSocket = GetSkeletonMeshComponent()->GetSocketByName(GetWeaponData().ProjectileSpawnSocket))
	{
		const FTransform SocketTransform = SkeletalMeshSocket->GetSocketTransform(GetSkeletonMeshComponent());
		const FVector ToTarget = HitVector - SocketTransform.GetLocation();
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = GetOwner();
			SpawnParameters.Instigator = Pawn;
			World->SpawnActor<AProjectile>(GetWeaponData().Projectile, SocketTransform.GetLocation(), ToTarget.Rotation(),
			                               SpawnParameters);
		}
	}
}
