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

void AWorkerUnit::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AWorkerUnit::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWorkerUnit, OwnExtractor);
	DOREPLIFETIME(AWorkerUnit, WorkplaceIndex);
	DOREPLIFETIME(AWorkerUnit, CurrentTool);
}

int AWorkerUnit::GetEfficiency_Implementation() const
{
	return 1;
}

void AWorkerUnit::SetOwnExtractor(AIndustrialBuilding* Extractor)
{
	OwnExtractor = Extractor;
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

void AWorkerUnit::BeginExtract(AGameResource* TargetResource)
{
	if (TargetResource)
	{
		ResourceActionType = TargetResource->ProcessAnimType;
		BeginProcessAction(ResourceActionType, TargetResource);
		float TargetTime = TargetResource->GetExtractionTime(this);
		GetWorld()->GetTimerManager().SetTimer(ActionTimer, [this, TargetResource]() {
			if (TargetResource)
			{
				Items[0] = TargetResource->Extract(this);
				Exp += TargetResource->GetExtractionExp(this);
			}
			EndExtract();
		}, TargetTime, false, TargetTime);
	}
}

void AWorkerUnit::EndExtract()
{
	EndAction(ResourceActionType);	
}