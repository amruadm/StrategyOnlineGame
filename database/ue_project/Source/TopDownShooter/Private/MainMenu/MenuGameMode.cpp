// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/MainMenu/MenuGameMode.h"
#include "Public/MainMenu/MenuPlayerController.h"
#include "Public/MainMenu/MenuHUD.h"



AMenuGameMode::AMenuGameMode()
{	
	HUDClass = AMenuHUD::StaticClass();
	PlayerControllerClass = AMenuPlayerController::StaticClass();
}