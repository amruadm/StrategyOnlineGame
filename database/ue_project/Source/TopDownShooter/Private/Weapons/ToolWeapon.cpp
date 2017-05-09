// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Weapons/ToolWeapon.h"


AWorkerUnit* AToolWeapon::GetOwnerWorker() const
{
	return Cast<AWorkerUnit>(Owner);
}

int AToolWeapon::GetEfficiency_Implementation() const
{
	return Efficiency;
}