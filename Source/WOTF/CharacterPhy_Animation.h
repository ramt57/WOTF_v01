// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterPhy_Animation.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WOTF_API UCharacterPhy_Animation : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterPhy_Animation();
	
	// Declare the custom function
	UFUNCTION(BlueprintCallable, Category = "PhysicalAnimation")
	void TogglePhysicalAnimation(bool Enable, FName BodyName, FName ProfileName) const;

private:
	UPROPERTY()
	class USkeletalMeshComponent* OwnerMesh;
	
	UPROPERTY(EditAnywhere)
	class UPhysicalAnimationComponent* PhysicalAnimationComponent;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
