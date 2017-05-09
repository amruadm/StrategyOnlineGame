// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/HUD/GameHUD.h"

bool AGameHUD::OnToggleHeroInventory_Implementation()
{
	return true;
}

void AGameHUD::DrawHUD()
{
	Super::DrawHUD();
}