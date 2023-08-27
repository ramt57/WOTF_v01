// Fill out your copyright notice in the Description page of Project Settings.


#include "WOTFGameInstance.h"

#include "WOTF/utils/FLogUtil.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemTypes.h"

void UWOTFGameInstance::Init()
{
	Super::Init();
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get(); OnlineSubsystem != nullptr)
	{
		const FString Name = OnlineSubsystem->GetSubsystemName().ToString();
		FLogUtil::Warning(Name);
		if (SessionPtr = OnlineSubsystem->GetSessionInterface(); SessionPtr.IsValid())
		{
			SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UWOTFGameInstance::OnCreateSessionComplete);
			SessionPtr->OnDestroySessionCompleteDelegates.
			            AddUObject(this, &UWOTFGameInstance::OnDestroySessionComplete);
			SessionPtr->OnFindSessionsCompleteDelegates.AddUObject(
				this, &UWOTFGameInstance::OnFindSessionsCompleteDelegates);
			SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UWOTFGameInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		FLogUtil::Warning(TEXT("Online Subsytem not found"));
	}
}

void UWOTFGameInstance::Host(const FName SessionName, const bool IsLan, const int NumOfPlayer) const
{
	if (SessionPtr.IsValid())
	{
		if (const auto ExistingSession = SessionPtr->GetNamedSession(SessionName); ExistingSession != nullptr)
		{
			SessionPtr->DestroySession(SessionName);
		}
		else
		{
			CreateSession(SessionName, IsLan, NumOfPlayer);
		}
	}
}

void UWOTFGameInstance::Join(const FName SessionName, const FString Address) const
{
	if (SessionPtr.IsValid() && SessionSearch.IsValid())
	{
		// Loop through the search results array to find the element with the matching session ID
		for (const FOnlineSessionSearchResult& SessionResult : SessionSearch->SearchResults)
		{
			if (SessionResult.Session.GetSessionIdStr() == Address)
			{
				// Found the session with the matching ID
				SessionPtr->JoinSession(0, SessionName, SessionResult);
			}
		}
	}
}

void UWOTFGameInstance::FindGameSessions(const bool IsLan)
{
	if (SessionPtr.IsValid())
	{
		SessionSearch = MakeShareable(new FOnlineSessionSearch());
		if (SessionSearch.IsValid())
		{
			SessionSearch->bIsLanQuery = IsLan;
			SessionSearch->MaxSearchResults = 1000;
			SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
			SessionPtr->FindSessions(0, SessionSearch.ToSharedRef());
		}
	}
}

void UWOTFGameInstance::CreateSession(const FName SessionName, const bool IsLan, const int NumOfPlayer) const
{
	if (SessionPtr.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = IsLan;
		SessionSettings.NumPublicConnections = NumOfPlayer;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.Set(Server_Name_Settings_Key, SessionName.ToString(),
		                    EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionPtr->CreateSession(0, SessionName, SessionSettings);
	}
}

void UWOTFGameInstance::OnCreateSessionComplete(const FName SessionName, const bool Success) const
{
	if (Success)
	{
		UEngine* Engine = GetEngine();
		if (!ensure(Engine != nullptr)) return;
		Engine->AddOnScreenDebugMessage(0, 2, FColor::Green,
		                                FString::Printf(TEXT("Hosting... %s"), *SessionName.ToString()));

		UWorld* World = GetWorld();
		if (!ensure(World != nullptr)) return;
		World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
	}
	else
	{
		FLogUtil::Warning(TEXT("Cannot create Session"));
	}
}

void UWOTFGameInstance::OnDestroySessionComplete(const FName SessionName, const bool Success) const
{
	if (Success)
	{
		CreateSession(SessionName, false, 4);
	}
	else
	{
		FLogUtil::Warning(TEXT("This Session cannot be destroyed"));
	}
}

void UWOTFGameInstance::OnFindSessionsCompleteDelegates(const bool Success) const
{
	if (Success && SessionSearch.IsValid())
	{
		TArray<FSessionServerData> ServerNames;
		FLogUtil::Warning(TEXT("Finding session complete"));
		for (const FOnlineSessionSearchResult& SessionResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Session names %s"), *SessionResult.GetSessionIdStr());
			FSessionServerData ServerData;
			ServerData.SessionStrId = SessionResult.GetSessionIdStr();
			ServerData.HostUserName = SessionResult.Session.OwningUserName;
			ServerData.MaxPlayers = SessionResult.Session.SessionSettings.NumPublicConnections;
			ServerData.CurrentPlayers = ServerData.MaxPlayers - SessionResult.Session.NumOpenPublicConnections;
			if (FString SessionName; SessionResult.Session.SessionSettings.Get(Server_Name_Settings_Key, SessionName))
			{
				ServerData.SessionName = SessionName;
			}
			else
			{
				ServerData.SessionName = FString("Unknown");
			}
			ServerNames.Add(ServerData);
		}
		// Broadcast the delegate to notify that ServerNames have been updated
		OnServerNamesUpdated.Broadcast(ServerNames);
	}
}

void UWOTFGameInstance::OnJoinSessionComplete(const FName SessionName,
                                              EOnJoinSessionCompleteResult::Type JoinSessionCompleteResult) const
{
	if (SessionPtr.IsValid())
	{
		FString Address;
		SessionPtr->GetResolvedConnectString(SessionName, Address);

		UEngine* Engine = GetEngine();
		if (!ensure(Engine != nullptr)) return;
		Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining... %s"), *Address));

		APlayerController* PlayerController = GetFirstLocalPlayerController();
		if (!ensure(PlayerController != nullptr)) return;

		PlayerController->ClientTravel(Address, TRAVEL_Absolute);
	}
}
