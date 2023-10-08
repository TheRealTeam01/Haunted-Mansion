// Fill out your copyright notice in the Description page of Project Settings.


#include "HMOverlay.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"

void UHMOverlay::ShowCrossHair()
{
	CrossHairCenter->SetVisibility(ESlateVisibility::Visible);
	CrossHairTop->SetVisibility(ESlateVisibility::Visible);
	CrossHairBottom->SetVisibility(ESlateVisibility::Visible);
	CrossHairLeft->SetVisibility(ESlateVisibility::Visible);
	CrossHairRight->SetVisibility(ESlateVisibility::Visible);
}

void UHMOverlay::HideCrossHair()
{
	CrossHairTop->SetVisibility(ESlateVisibility::Hidden);
	CrossHairBottom->SetVisibility(ESlateVisibility::Hidden);
	CrossHairRight->SetVisibility(ESlateVisibility::Hidden);
	CrossHairLeft->SetVisibility(ESlateVisibility::Hidden);
	CrossHairCenter->SetVisibility(ESlateVisibility::Hidden);
}

void UHMOverlay::PlayBlink()
{
	if (Blink && NoAmmoText)
	{
		NoAmmoText->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(Blink);
	}
}

void UHMOverlay::ShowBloodSplatter(float Health)
{
	if (Health <= 80.f && BloodSplatterAnim)
	{
		BloodSplatter->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(BloodSplatterAnim, 0.f, 1);
	}
}
