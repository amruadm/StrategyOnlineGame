// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TOPDOWNSHOOTER_API AGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	virtual void DrawHUD() override;

	UFUNCTION(BlueprintNativeEvent, Category="Input")
	bool OnToggleHeroInventory();
};
