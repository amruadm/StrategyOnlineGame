// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Units/WorkerUnit.h"
#include "Public/Gameplay/WorkerToolItem.h"
#include "Public/Resource/GameResource.h"
#include "Public/Resource/ResourceItem.h"
#include "Public/Buildings/IndustrialBuilding.h"


AWorkerUnit::AWorkerUnit()
{
	CurrentTool = nullptr;
	WeaponAttachmentComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponAttachment"));
	WeaponAttachmentComponent->AttachTo(GetMesh());
	PrimaryActorTick.bCanEverTick = true;
}

void AWorkerUnit::BeginPlay()
{
	Super::BeginPlay();

	if(!CurrentTool)
		EquipDefaultTool();
}

void AWorkerUnit::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWorkerUnit, OwnExtractor);
	DOREPLIFETIME(AWorkerUnit, WorkplaceIndex);
	DOREPLIFETIME(AWorkerUnit, CurrentTool);
	DOREPLIFETIME(AWorkerUnit, TargetResource);
}

int AWorkerUnit::GetEfficiency_Implementation() const
{
	return 1;
}

void AWorkerUnit::SetOwnExtractor(AIndustrialBuilding* Extractor)
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

void AWorkerUnit::EquipTool(TSubclassOf<UWorkerToolItem> NewTool)
{
	if (!NewTool) return;
	CurrentTool = NewTool;
}

void AWorkerUnit::EquipDefaultTool()
{
	CurrentTool = DefaultToolClass;
}

void AWorkerUnit::OnRep_CurrentTool()
{
	UStaticMesh* TargetMesh = nullptr;
	if (CurrentTool)
	{
		if (UWorkerToolItem* ItemObject = CurrentTool->GetDefaultObject<UWorkerToolItem>())
		{
			TargetMesh = ItemObject->AttachmentMesh;			
		}
	}
	WeaponAttachmentComponent->SetStaticMesh(TargetMesh);
}

void AWorkerUnit::BeginExtract()
{

}

void AWorkerUnit::EndExtract()
{

}