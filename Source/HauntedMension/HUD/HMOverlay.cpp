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
	if (Blink && BlinkText)
	{
		BlinkText->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(Blink);
	}
}

void UHMOverlay::PlayPickUp(FText Text, UTexture2D* Image)
{
	if (PickUp && PickUpText)
	{

		PickUpImage->SetBrushFromTexture(Image);
		PickUpImage->SetVisibility(ESlateVisibility::Visible);
		PickUpText->SetText(Text);
		PickUpText->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(PickUp);
	}
}

void UHMOverlay::SetBlinkText(FText Text)
{
	BlinkText->SetText(Text);
}

