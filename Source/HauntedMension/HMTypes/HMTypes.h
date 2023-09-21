#pragma once

UENUM(BlueprintType)
enum class EFlashLightState : uint8
{
	EFS_UnEquippedFlashLight UMETA(DisplayName = "Unequipped FlashLight"),
	EFS_EquippedFlashLight UMETA(DisplayName = "Equipped FlashLight")
};