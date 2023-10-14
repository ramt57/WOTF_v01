// Fill out your copyright notice in the Description page of Project Settings.
#include "ItemBase.h"

#include "EItemState.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "WOTF/TP_ThirdPerson/CharacterInterface.h"


// Sets default values
AItemBase::AItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMeshComponent->SetupAttachment(StaticMeshComponent);
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(StaticMeshComponent);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	WidgetComponent->SetupAttachment(GetRootComponent());
	WidgetComponent->SetVisibility(false);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawAtDesiredSize(true);
	WidgetComponent->SetDrawSize(FVector2D(300.f, 120.f));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->SetSphereRadius(150.f);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	/* Sets collision response and overlap events only when we are a server */
	if (HasAuthority())
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnSphereOverlap);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnSphereOverlapEnd);
	}
}

void AItemBase::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		ICharacterInterface::Execute_SetOverlappedItemBase(OtherActor, this);
	}
}

void AItemBase::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		ICharacterInterface::Execute_SetOverlappedItemBase(OtherActor, nullptr);
	}
}

void AItemBase::OnRep_ItemState() const
{
	InvalidateItemState();
}

void AItemBase::InvalidateItemState() const
{
	switch (ItemState)
	{
	case EItemState::Initial:
		{
			/* Enable Collision */
			SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			break;
		}
	case EItemState::Equipped:
		{
			WidgetComponent->SetVisibility(false);
			/* Ignore & Disable collision for all channel */
			SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		}
	case EItemState::Dropped:
		{
			break;
		}
	case EItemState::Pickup: break;
	case EItemState::AddedToInventory: break;
	case EItemState::MaxCount: break;
	default: ;
	}
}

void AItemBase::SetItemState(const EItemState State)
{
	this->ItemState = State;
	InvalidateItemState();
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemBase::ToggleVisibilityOfItemPickupWidget_Implementation(const bool Visibility)
{
	if (WidgetComponent)
	{
		WidgetComponent->SetVisibility(Visibility);
	}
}

void AItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AItemBase, ItemState);
}
