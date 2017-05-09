// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Buildings/Building.h"
#include "Public/Units/SimpleWorkerUnit.h"
#include "Public/Resource/ResourceItem.h"


// Sets default values
ABuilding::ABuilding()
{
	BoundingBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoundingBox"));

	BuildingMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BuildingMesh"));
	//BuildingMeshComponent->SetSkeletalMesh(BuildingMesh.Get(), true);
	BuildingMeshComponent->AttachTo(BoundingBoxComponent);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	SetLevel(1);
}

// Called every frame
void ABuilding::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ABuilding::SetLevel(int NewLevel)
{
	Level = NewLevel;
	if (BuildResources.IsValidIndex(Level))
		CurrentBuildResources = BuildResources[Level - 1].Resources;
	
	OnLevelChanged(NewLevel);
}

void ABuilding::OnLevelChanged_Implementation(int NewLevel)
{
	
}

void ABuilding::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABuilding, TeamNum);
	DOREPLIFETIME(ABuilding, Level);
	DOREPLIFETIME(ABuilding, Workers);
	DOREPLIFETIME(ABuilding, CurrentBuildResources);
	DOREPLIFETIME(ABuilding, Health);
}

USkeletalMesh* ABuilding::GetBuildingMesh()
{
	if (BuildingMeshComponent)
	{
		return BuildingMeshComponent->SkeletalMesh;
	}
	return nullptr;
}

void ABuilding::ProcessBuild(ASimpleWorkerUnit* Unit)
{
	for (FItemCeil & item : CurrentBuildResources)
	{
		int c = Unit->TakeItemsOfClass(item.ItemClass, 1);
		if (c == 1) return;
	}
}

FItemCeil ABuilding::GetFirstNeeded()
{

	for (auto item : CurrentBuildResources)
	{
		if (item.Count > 0) return item;
	}
	return FItemCeil::EmptyItem;
}

bool ABuilding::HasCompleted() const
{
	bool res = true;
	for (auto item : CurrentBuildResources)
	{
		res &= item.Count <= 0;
	}
	return res;
}

bool ABuilding::AddWorker(ASimpleWorkerUnit* Unit)
{
	if (Workers.Num() < MaxWorkerCount)
	{
		int i = 0;
		if(!ContainsWorker(Unit))
			Workers.Add(Unit);
		return true;
	}
	return false;
}

void ABuilding::RemoveWorker(ASimpleWorkerUnit* Unit)
{
	Workers.Remove(Unit);
}

bool ABuilding::ContainsWorker(ASimpleWorkerUnit* Unit)
{
	int i;
	return Workers.Find(Unit, i);
}

bool ABuilding::CanWorkerAdded() const
{
	return (Workers.Num() < MaxWorkerCount) && !HasCompleted();
}

float ABuilding::GetMaxHealth() const
{
	return GetClass()->GetDefaultObject<ABuilding>()->Health;
}

float ABuilding::GetCurrentHealth() const
{
	return Health;
}

void ABuilding::PlaceUnit(AGameUnit* Unit){	return; }

TArray<AGameUnit*> ABuilding::GetPlacedUnits() const { return TArray<AGameUnit*>(); }

bool ABuilding::CanBePlaced(AGameUnit* Unit) const { return false; }

/*
	!!!!!!!!!!!!!!!!!!! ƒŒœ»À»“‹!!!
*/
bool ABuilding::CheckWorkerNecessity(ASimpleWorkerUnit* Worker) const
{
	return false;
}

void ABuilding::Upgrade()
{
	if (Level < MaxLevel)
	{
		SetLevel(Level + 1);
	}
}

int ABuilding::GetGradeCost_Implementation() const
{
	if (GradeCostList.IsValidIndex(Level))
	{
		return GradeCostList[Level];
	}
	return 0;
}