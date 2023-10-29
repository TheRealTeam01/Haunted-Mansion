// Fill out your copyright notice in the Description page of Project Settings.


#include "HMDeath.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"

void UHMDeath::PlayDeathAnim()
{
	if (DeathAnim)
	{
		PlayAnimation(DeathAnim, 0.f ,1);
	}
}
