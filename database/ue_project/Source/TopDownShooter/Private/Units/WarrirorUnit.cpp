// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Units/WarrirorUnit.h"
#include "AI/Navigation/NavigationSystem.h"



void AWarrirorUnit::SetTargetPoint_Implementation(const struct FCommandTarget & Target)
{
	if (ITeamObjectInterface* TeamObject = Cast<ITeamObjectInterface>(Target.Controller))
	{
		if (TeamObject->GetTeamNum() == TeamNum)
		{
			FVector DestLocation = Target.TargetActor ? Target.TargetActor->GetActorLocation() : Target.TargetPosition;
			UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
			float const Distance = FVector::Dist(DestLocation, GetActorLocation());

			// We need to issue move command only if far enough in order for walk animation to play correctly
			if (NavSys && (Distance > TARGET_MOVE_DISTANCE))
			{
				NavSys->SimpleMoveToLocation(GetController(), DestLocation);
			}
		}
	}
}