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
	UStaticMeshComponent* StaticMeshComponent;

public:
	FORCEINLINE UStaticMeshComponent* GetStaticMeshComponent() const
	{
		return StaticMeshComponent;
	}
	FORCEINLINE USkeletalMeshComponent* GetSkeletonMeshComponent() const
	{
		return SkeletalMeshComponent;
	}
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta=(AllowPrivateAccess = "true"))
	class UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta=(AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(ReplicatedUsing= OnRep_ItemState, VisibleAnywhere)
	EItemState ItemState;

	UFUNCTION()
	void OnRep_ItemState() const;
	
	UPROPERTY(VisibleAnywhere)
	EItemType ItemType;
protected:
	virtual void InvalidateItemState() const;
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
	FORCEINLINE EItemState GetItemState() const
	{
		return ItemState;
	}
private:
	virtual void ToggleVisibilityOfItemPickupWidget_Implementation(bool Visibility) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
