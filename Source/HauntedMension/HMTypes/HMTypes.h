#pragma once

UENUM(BlueprintType)
enum class EFlashLightState : uint8
{
	EFS_UnEquippedFlashLight UMETA(DisplayName = "Unequipped FlashLight"),
	EFS_EquippedFlashLight UMETA(DisplayName = "Equipped FlashLight")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Aiming UMETA(DisplayName = "Aiming"),
	EAS_Reloading UMETA(DisplayName = "Reloading"),
	EAS_PickUp UMETA(DisplayName = "Pick Up"),
	
	EAS_MAX UMETA(DisplayName = "Default MAX")
};

UENUM(BlueprintType)
enum class EKeyState : uint8
{
	EKS_UnEquippedKey UMETA(DisplayName = "Unequipped Key"),
	EKS_EquippedKey UMETA(DisplayName = "Equipped Key")
};

UENUM(BlueprintType)
enum class EDeathState : uint8
{
	EDS_Alive UMETA(DisplayName = "Alive"),
	EDS_Death UMETA(DisplayName = "Death")
};
UENUM(BlueprintType)
enum class ESevarogState : uint8
{
	E_Undefine	UMETA(DisplayName = "Sevarog Undefine"),
	E_Idle		UMETA(DisplayName = "Sevarog Idle"),
	E_Patrol	UMETA(DisplayName = "Sevarog Patrol"),
	E_Chase		UMETA(DisplayName = "Sevarog Chasing"),
	E_Attack	UMETA(DisplayName = "Sevarog Attack"),
	E_Die		UMETA(DisplayName = "Sevarog Die")
};