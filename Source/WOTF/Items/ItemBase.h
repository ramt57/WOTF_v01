// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInterface.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

enum class EItemType : uint8;
enum class EItemState : uint8;

UCLASS()
class WOTF_API AItemBase : public AActor, public IItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	UFUNCTION()
	virtual void OnSphereOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex
	);
	

public:
	UPROPERTY()
	bool bCharacterCanStartLineTrace = false;
	
	UFUNCTION()
	FORCEINLINE bool GetWidgetVisibility()
	{
		return WidgetComponent->IsVisible();
	}
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta=(AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMesh;

public:
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMesh() const
	{
		return SkeletalMesh;
	}

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta=(AllowPrivateAccess = "true"))
	class UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta=(AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent;

	UPROPERTY(ReplicatedUsing= OnRep_ItemState, VisibleAnywhere)
	EItemState ItemState;

	UFUNCTION()
	void OnRep_ItemState() const;
	void InvalidateItemState() const;

	UPROPERTY(EditAnywhere)
	EItemType ItemType;

public:
	FORCEINLINE EItemType& GetItemType()
	{
		return ItemType;
	}

	void SetItemType(const EItemType Type)
	{
		this->ItemType = Type;
	}
	
	void SetItemState(const EItemState ItemState);
private:
	virtual void ToggleVisibilityOfItemPickupWidget_Implementation(bool Visibility) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
