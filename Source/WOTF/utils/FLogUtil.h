#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUtil, Log, All);

class FLogUtil
{
public:
	static void Log(const FString& Message);
	static void Warning(const FString& Message);
	static void Error(const FString& Message);
};
