#include "CharacterPhy_Animation.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"

UCharacterPhy_Animation::UCharacterPhy_Animation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterPhy_Animation::TogglePhysicalAnimation(const bool Enable, const FName BodyName,
                                                      const FName ProfileName) const
{
	if (PhysicalAnimationComponent && OwnerMesh && Enable)
	{
		if(Enable)
		{
			OwnerMesh->SetAllBodiesBelowSimulatePhysics(BodyName, Enable, false);
			PhysicalAnimationComponent->ApplyPhysicalAnimationProfileBelow(BodyName, ProfileName);
		}
		else
		{
			PhysicalAnimationComponent->ApplyPhysicalAnimationProfileBelow(BodyName, FName("NoProfileName"));
			OwnerMesh->ResetAllBodiesSimulatePhysics();
		}
	}
}

void UCharacterPhy_Animation::BeginPlay()
{
	Super::BeginPlay();
	// Get the owner of this component
	if (const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		// Cast the mesh component to USkeletalMeshComponent if it's a Skeletal mesh
		OwnerMesh = Cast<USkeletalMeshComponent>(OwnerCharacter->GetMesh());
		if (OwnerMesh)
		{
			// Create a Physical Animation Component
			PhysicalAnimationComponent = NewObject<UPhysicalAnimationComponent>(GetOwner());
			PhysicalAnimationComponent->SetSkeletalMeshComponent(OwnerMesh);
		}
	}
}
