#pragma once

UENUM(BlueprintType)
enum class EFlashLightState : uint8
{
	EFS_UnEquippedFlashLight UMETA(DisplayName = "Unequipped FlashLight"),
	EFS_EquippedFlashLight UMETA(DisplayName = "Equipped FlashLight")
};

UENUM(BlueprintType)
enum class ESevarogState : uint8
{
	E_Idle = 0		UMETA(DisplayName = "Idle"),
	E_Patrol	UMETA(DisplayName = "Patrol"),
	E_Chase		UMETA(DisplayName = "Chase"),
	E_Attack	UMETA(DisplayName = "Attack"),
	E_Die		UMETA(DisplayName = "Die")
};