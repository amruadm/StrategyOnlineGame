// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Units/WorkerUnit.h"
#include "Public/Weapons/ToolWeapon.h"
#include "Public/Resource/GameResource.h"
#include "Public/Resource/ResourceItem.h"
#include "Public/Buildings/BuildingExtractor.h"


AWorkerUnit::AWorkerUnit()
{

}

void AWorkerUnit::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWorkerUnit, OwnExtractor);
	DOREPLIFETIME(AWorkerUnit, WorkplaceIndex);
}

int AWorkerUnit::GetEfficiency_Implementation() const
{
	return 1;
}

void AWorkerUnit::SetOwnExtractor(ABuildingExtractor* Extractor)
{
	if (!Extractor)
	{
		OwnExtractor = nullptr;
		return;
	}

	if (Extractor->CanBePlaced(this))
	{
		OwnExtractor = Extractor;
		Extractor->AddWorker(this);		
	}
}

void AWorkerUnit::EquipTool(AToolWeapon* NewTool)
{
	if (!NewTool) return;
	if (CurrentTool) CurrentTool->Destroy();
	CurrentTool = NewTool;
	CurrentTool->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocketName);
	CurrentTool->Owner = this;
}

void AWorkerUnit::EquipDefaultTool()
{
	AToolWeapon* tool = GetWorld()->SpawnActor<AToolWeapon>(DefaultToolClass);
	if (tool)
	{
		EquipTool(tool);
	}
}