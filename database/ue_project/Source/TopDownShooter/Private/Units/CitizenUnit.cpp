// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Units/CitizenUnit.h"
#include "Public/Buildings/BuildingHouse.h"
#include "TopDownShooterPlayerController.h"
#include "UnrealNetwork.h"

void ACitizenUnit::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACitizenUnit, OwnHouse);
}

ACitizenUnit* ACitizenUnit::TakeProfession(TSubclassOf<ACitizenUnit> NewProfession)
{
	if (Role == ROLE_Authority)
	{
		ACitizenUnit* unit = GetWorld()->SpawnActor<ACitizenUnit>(NewProfession);
		if (unit)
		{
			unit->SetActorLocation(GetActorLocation());
			unit->SetActorRotation(GetActorRotation());
			unit->SetTeamNum(GetTeamNum());
			unit->Exp = Exp;
			if (OwnHouse)
			{
				OwnHouse->Citizens.Remove(this);
				OwnHouse->Citizens.Add(unit);
				unit->OwnHouse = OwnHouse;
			}			

			Destroy();
			return unit;
		}
	}
	return nullptr;
}