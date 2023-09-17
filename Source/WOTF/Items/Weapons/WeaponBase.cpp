// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "WOTF/TP_ThirdPerson/TP_ThirdPersonCharacter.h"

AWeaponBase::AWeaponBase()
{

}

void AWeaponBase::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	Super::OnSphereOverlapEnd(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex);
	ATP_ThirdPersonCharacter* ThirdPersonCharacter = Cast<ATP_ThirdPersonCharacter>(OtherActor);
	if(ThirdPersonCharacter)
	{
		ThirdPersonCharacter->SetPrimaryWeapon(this);
	}
}
