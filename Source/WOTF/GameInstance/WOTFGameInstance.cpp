// Fill out your copyright notice in the Description page of Project Settings.


#include "WOTFGameInstance.h"

#include "WOTF/utils/FLogUtil.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

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
		}
	}
	else
	{
		FLogUtil::Warning(TEXT("Online Subsytem not found"));
	}
}

void UWOTFGameInstance::Host(const FName SessionName) const
{
	if (SessionPtr.IsValid())
	{
		if (const auto ExistingSession = SessionPtr->GetNamedSession(SessionName); ExistingSession != nullptr)
		{
			SessionPtr->DestroySession(SessionName);
		}
		else
		{
			CreateSession(SessionName);
		}
	}
}

void UWOTFGameInstance::CreateSession(const FName SessionName) const
{
	if (SessionPtr.IsValid())
	{
		const FOnlineSessionSettings SessionSettings;
		SessionPtr->CreateSession(0, SessionName, SessionSettings);
	}
}

void UWOTFGameInstance::OnCreateSessionComplete(FName SessionName, const bool Success) const
{
	if (Success)
	{
		UEngine* Engine = GetEngine();
		if (!ensure(Engine != nullptr)) return;
		Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

		UWorld* World = GetWorld();
		if (!ensure(World != nullptr)) return;
		World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
	}
	else
	{
		FLogUtil::Warning(TEXT("Cannot create Session"));
	}
}

void UWOTFGameInstance::OnDestroySessionComplete(FName SessionName, const bool Success) const
{
	if (Success)
	{
		CreateSession(SessionName);
	}
	else
	{
		FLogUtil::Warning(TEXT("This Session cannot be destroyed"));
	}
}
