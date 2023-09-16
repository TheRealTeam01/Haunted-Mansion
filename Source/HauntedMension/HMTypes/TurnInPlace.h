#pragma once

UENUM(BlueprintType)
enum class ETurnInPlace : uint8
{
	ETIP_TurnLeft UMETA(DisplayName = "Turning Left"),
	ETIP_TurnRight UMETA(DisplayName = "Turning Right"),
	ETIP_NotTurning UMETA(DisplayName = "Not Turning"),

	ETIP_MAX UMETA(DisplayName = "Default MAx")
};