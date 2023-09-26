// Fill out your copyright notice in the Description page of Project Settings.


#include "HMOverlay.h"
#include "Components/Image.h"

void UHMOverlay::ShowCrossHair()
{
	CrossHair->SetVisibility(ESlateVisibility::Visible);
}

void UHMOverlay::HideCrossHair()
{
	CrossHair->SetVisibility(ESlateVisibility::Hidden);
}
