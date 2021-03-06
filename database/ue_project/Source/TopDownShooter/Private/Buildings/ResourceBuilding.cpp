// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Buildings/IndustrialBuilding.h"
#include "Public/Buildings/ResourceBuilding.h"
#include "Public/Units/WorkerUnit.h"

void AResourceBuilding::BeginPlay()
{
	Super::BeginPlay();
	ResizeWorkplaces();
}

int AResourceBuilding::GetWorkplacesNum_Implementation() const
{
	return Level;
}

void AResourceBuilding::AddWorker(AWorkerUnit* Unit)
{
	for (int i = 0; i < Workplaces.Num(); i++)
	{
		FResourceWorkplace & workplace = Workplaces[i];
		if (!workplace.Worker)
		{
			workplace.Worker = Unit;
			Unit->SetWorkplaceIndex(i);
			Unit->SetOwnExtractor(this);
			return;
		}
	}
}

bool AResourceBuilding::ContainsWorker(class AWorkerUnit* Unit) const
{
	for (const FResourceWorkplace & work : Workplaces)
	{
		if (work.Worker == Unit) return true;
	}
	return false;
}

bool AResourceBuilding::CanWorkerAdded() const
{
	for (const FResourceWorkplace & work : Workplaces)
	{
		if (!work.Worker)
		{
			return true;
		}
	}
	return false;
}

void AResourceBuilding::PlaceUnit(class AGameUnit* Unit)
{

}

void AResourceBuilding::OnUnplace(class AGameUnit* Unit)
{

}

TArray<AGameUnit*> AResourceBuilding::GetPlacedUnits() const
{
	TArray<AGameUnit*> Result;
	for (const FResourceWorkplace & work : Workplaces)
	{
		if (work.Worker)
		{
			Result.Add(Cast<AGameUnit>(work.Worker));
		}
	}
	return Result;
}

bool AResourceBuilding::CanBePlaced(class AGameUnit* Unit) const
{
	return false;
}

void AResourceBuilding::ResizeWorkplaces()
{
	Workplaces.SetNum(GetWorkplacesNum());
}