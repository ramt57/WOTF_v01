#pragma once

#include "CoreMinimal.h"
#include "SessionServerData.generated.h"

USTRUCT(BlueprintType)
struct FSessionServerData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly)
	FString SessionName;
	UPROPERTY(BlueprintReadOnly)
	FString SessionStrId;
	UPROPERTY(BlueprintReadOnly)
	FString HostUserName;
	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayers;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayers;
};