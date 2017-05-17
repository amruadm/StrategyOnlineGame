// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Units/WarrirorUnit.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Public/AI/UnitAIController.h"



void AWarrirorUnit::SetTargetPoint_Implementation(const struct FCommandTarget & Target)
{
	if (ITeamObjectInterface* TeamObject = Cast<ITeamObjectInterface>(Target.Controller))
	{
		if (TeamObject->GetTeamNum() == TeamNum)
		{
			if (AUnitAIController* Controller = Cast<AUnitAIController>(GetController()))
			{
				Controller->SetTarget(Target);
			}
		}
	}
}