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
	
	EAS_MAX UMETA(DisplayName = "Default MAX")
};