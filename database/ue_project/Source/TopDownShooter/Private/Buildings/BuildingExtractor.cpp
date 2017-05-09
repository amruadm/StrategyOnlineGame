// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Buildings/BuildingExtractor.h"
#include "Public/Units/WorkerUnit.h"
#include "Public/Resource/ResourceItem.h"

void ABuildingExtractor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABuildingExtractor, Workplaces);
}

int ABuildingExtractor::GetWorkplacesNum_Implementation() const
{
	return Level;
}

void ABuildingExtractor::BeginConversion(AWorkerUnit* Unit)
{
	if (!Unit) return;

	for (int i = 0; i < Workplaces.Num(); i++)
	{
		FExtractorWorkplace & workplace = Workplaces[i];
	}
	for (FExtractorWorkplace & workplace : Workplaces)
	{
		if (workplace.Worker == Unit)
		{
			FTimerDelegate SpawnDelegate = FTimerDelegate::CreateLambda([=]()
			{
				ProcessConvertion(Unit);
			});
			GetWorld()->GetTimerManager().SetTimer(workplace.Timer, SpawnDelegate, GetTiming(Unit), false);
			break;
		}
	}	
}

void ABuildingExtractor::AddWorker(class AWorkerUnit* Unit)
{
	for (int i = 0; i < Workplaces.Num(); i++)
	{
		FExtractorWorkplace & workplace = Workplaces[i];
		if (!workplace.Worker)
		{
			Unit->SetWorkplaceIndex(i);
			return;
		}
	}
}

void ABuildingExtractor::ProcessConvertion(AWorkerUnit* Unit)
{
	
}

float ABuildingExtractor::GetTiming(AWorkerUnit* Worker) const
{
	for (const FExtractorWorkplace & workplace : Workplaces)
	{
		if (workplace.Worker == Worker)
		{
			int index = 0;
			if (workplace.ItemIndices.IsValidIndex(0))
			{
				index = workplace.ItemIndices[0];
				if (Items.IsValidIndex(index))
				{
					const FExtractorItem & item = Items[index];
					const float a = FMath::Clamp((Worker->Exp - item.MinExp) / item.MinExp, 0.0f, 1.0f);
					return FMath::Max(
						item.Timing - FMath::Lerp(0.0f, item.Timing / EXTRACTOR_EXP_RATE_LIMIT, a),
						EXTRACTOR_MIN_TIMING
					);
				}
			}
			break;
		}
	}
	return 0.0f;
}

void ABuildingExtractor::PlaceUnit(AGameUnit* Unit)
{
	if (!Unit) return;
	Unit->SetActorHiddenInGame(true);
}

void ABuildingExtractor::OnUnplace(AGameUnit* Unit)
{
	if (!Unit) return;
	Unit->SetActorHiddenInGame(false);
}

TArray<AGameUnit*> ABuildingExtractor::GetPlacedUnits() const
{
	TArray<AGameUnit*> workers;
	for (auto item : Workplaces)
	{
		if (item.Worker)
			workers.Add(item.Worker);
	}
	return workers;
}

bool ABuildingExtractor::CanBePlaced(AGameUnit* Unit) const
{
	AWorkerUnit* worker = Cast<AWorkerUnit>(Unit);
	if (worker)
	{
		if (worker->GetClass() != WorkerClass) return false;

		for (auto work : Workplaces)
		{
			if (work.Worker == worker || !work.Worker)
			{
				return true;
				break;
			}
		}
	}
	return false;
}