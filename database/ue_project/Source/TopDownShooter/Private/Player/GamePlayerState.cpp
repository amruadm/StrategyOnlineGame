// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "GamePlayerState.h"
#include "UnrealNetwork.h"

AGamePlayerState::AGamePlayerState()
{
	bReplicates = true;
}

void AGamePlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGamePlayerState, TeamNum);
}