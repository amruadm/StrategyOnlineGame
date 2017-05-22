// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Buildings/IndustrialBuilding.h"
#include "Public/Units/WorkerUnit.h"

void AIndustrialBuilding::BeginPlay()
{
	Super::BeginPlay();

	ResizeWorkplaces();
}

int AIndustrialBuilding::GetWorkplacesNum_Implementation() const
{
	return 0;
}

void AIndustrialBuilding::TakeCitizenToWork(ACitizenUnit* Unit)
{
	if (Unit)
	{
		if (AWorkerUnit* Worker = Cast<AWorkerUnit>(Unit->TakeProfession(WorkerClass)))
		{
			AddWorker(Worker);
		}
	}
}

void AIndustrialBuilding::ResizeWorkplaces()
{

}