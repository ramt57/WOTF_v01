// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "WOTFGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WOTF_API UWOTFGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	virtual void Init() override;
	IOnlineSessionPtr SessionPtr;
	void CreateSession(FName SessionName) const;
	void OnCreateSessionComplete(FName SessionName, bool Success) const;
	void OnDestroySessionComplete(FName SessionName, bool Success) const;
public:
	UFUNCTION(BlueprintCallable, Category="Multiplayer")
	void Host(FName SessionName) const;
};
