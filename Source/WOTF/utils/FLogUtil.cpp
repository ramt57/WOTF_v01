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
