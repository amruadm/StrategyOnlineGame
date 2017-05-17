// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Units/GameUnit.h"
#include "Public/AI/UnitAIController.h"
#include "Classes/Navigation/CrowdFollowingComponent.h"

AUnitAIController::AUnitAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{

}

void AUnitAIController::SetTarget(const FCommandTarget & Target)
{
	if (AGameUnit* Unit = GetControlledUnit())
	{
		if (ITeamObjectInterface* TeamObject = Cast<ITeamObjectInterface>(Target.Controller))
		{
			if (TeamObject->GetTeamNum() == Unit->GetTeamNum())
			{
				CommandTarget = Target;
			}
		}
	}
}

UCrowdFollowingComponent* AUnitAIController::GetCrowdComponent() const
{
	return Cast<UCrowdFollowingComponent>(PathFollowingComponent);
}