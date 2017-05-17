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
	if (HasCompleted())
	{
		OnBuildingComplete();
	}
}

// Called every frame
void ABuilding::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ABuilding::SetLevel(int NewLevel)
{
	Level = NewLevel;
	
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
	DOREPLIFETIME(ABuilding, BuildResources);
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
	if (HasCompleted()) return;
	for (FBuildingItemCeil & Item : BuildResources)
	{
		if (Item.CurrentCount >= Item.Count) continue;
		int c = Unit->TakeItemsOfClass(Item.ItemClass, 1);
		if (c == 1)
		{
			Item.CurrentCount++;
		}
	}
	if (HasCompleted())
	{
		Workers.Empty();
		OnBuildingComplete();
	}
}

FItemCeil ABuilding::GetFirstNeeded()
{
	for (const FBuildingItemCeil & Item : BuildResources)
	{
		if (Item.CurrentCount < Item.Count)
		{
			return Item;
		}
	}
	
	return FItemCeil::EmptyItem;
}

bool ABuilding::HasCompleted() const
{
	return GetBuildProgress() >= 1.0f;
}

bool ABuilding::AddWorker(ASimpleWorkerUnit* Unit)
{
	if (!Unit) return false;
	if (CanWorkerAdded() && Unit->GetTeamNum() == TeamNum)
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
	!!!!!!!!!!!!!!!!!!! ÄÎÏÈËÈÒÜ!!!
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

int ABuilding::GetRequiredResourceCount(TSubclassOf<UItem> ItemClass) const
{
	for (auto Item : BuildResources)
	{
		if (Item.ItemClass == ItemClass)
			return Item.Count - Item.CurrentCount;
	}
	return 0;
}

void ABuilding::Selected_Implementation()
{
	if (BuildingMeshComponent)
	{
		BuildingMeshComponent->SetRenderCustomDepth(true);
		if (ITeamObjectInterface* TeamObject = Cast<ITeamObjectInterface>(GetWorld()->GetFirstPlayerController()))
		{
			if (TeamObject->GetTeamNum() == TeamNum)
			{
				BuildingMeshComponent->SetCustomDepthStencilValue(STENCIL_FRIENDLY);
				return;
			}
		}
		BuildingMeshComponent->SetCustomDepthStencilValue(STENCIL_ENEMY);
	}
}

void ABuilding::Unselected_Implementation()
{
	if (BuildingMeshComponent)
	{
		BuildingMeshComponent->SetRenderCustomDepth(false);
	}
}

FControlData ABuilding::GetUIData_Implementation()
{
	FControlData Result;
	Result.Queues.bGroupEnabled = false;
	Result.Settings.bGroupEnabled = false;
	Result.Storage.bGroupEnabled = false;
	Result.Trading.bGroupEnabled = false;
	return Result;
}

float ABuilding::GetBuildProgress() const
{
	int NeededCount = 0;
	int BuildedCount = 0;
	for (auto Item : BuildResources)
	{
		BuildedCount += Item.CurrentCount;
		NeededCount += Item.Count;
	}
	if (NeededCount == 0) return 1.0f;
	return (float)BuildedCount / (float)NeededCount;
}
