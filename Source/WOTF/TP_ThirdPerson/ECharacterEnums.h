#pragma once

UENUM(BlueprintType)
enum class ETurnInPlace : uint8
{
	ETIP_CENTER UMETA(DisplayName = "Do not turn"),
	ETIP_LEFT UMETA(DisplayName = "Turn Left"),
	ETIP_RIGHT UMETA(DisplayName = "Turn Right"),
};
