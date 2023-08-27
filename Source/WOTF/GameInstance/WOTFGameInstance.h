// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "../Session/SessionServerData.h"
#include "WOTFGameInstance.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerNamesUpdatedDelegate, const TArray<FSessionServerData>&, UpdatedServerNames);
/**
 * 
 */
UCLASS()
class WOTF_API UWOTFGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	inline const static FName Server_Name_Settings_Key = FName(TEXT("SERVER_NAME"));
	virtual void Init() override;
	IOnlineSessionPtr SessionPtr;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	void CreateSession(FName SessionName, bool IsLan, int NumOfPlayer) const;
	void OnCreateSessionComplete(FName SessionName, bool Success) const;
	void OnDestroySessionComplete(FName SessionName, bool Success) const;
	void OnFindSessionsCompleteDelegates(bool Success) const;
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinSessionCompleteResult) const;
public:
	UPROPERTY(BlueprintAssignable, Category = "Server Names")
	FServerNamesUpdatedDelegate OnServerNamesUpdated;
	
	UFUNCTION(BlueprintCallable, Category="Multiplayer")
	void Host(FName SessionName, bool IsLan, int NumOfPlayer) const;
	UFUNCTION(BlueprintCallable, Category="Multiplayer")
	void Join(FName SessionName, FString Address) const;
	UFUNCTION(BlueprintCallable, Category="Multiplayer")
	void FindGameSessions(bool IsLan);
};
