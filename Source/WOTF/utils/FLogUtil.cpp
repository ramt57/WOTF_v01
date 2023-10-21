#include "FLogUtil.h"

DEFINE_LOG_CATEGORY(LogUtil);

void FLogUtil::Log(const FString& Message)
{
	UE_LOG(LogUtil, Log, TEXT("%s"), *Message);
}

void FLogUtil::Warning(const FString& Message)
{
	UE_LOG(LogUtil, Warning, TEXT("%s"), *Message);
}

void FLogUtil::Error(const FString& Message)
{
	UE_LOG(LogUtil, Error, TEXT("%s"), *Message);
}

void FLogUtil::PrintString(const FString& Message)
{
	// Display the message on screen
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, Message);
	}
}
