// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Units/SimpleWorkerUnit.h"
#include "Public/Buildings/Building.h"
#include "Public/Resource/ResourceItem.h"


// Sets default values
ABuilding::ABuilding()
{
	BoundingBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoundingBox"));

	BuildingMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BuildingMesh"));
	//BuildingMeshComponent->SetSkeletalMesh(BuildingMesh.Get(), true);
	BuildingMeshComponent->AttachTo(BoundingBoxComponent);

	BuildingMeshComponent->bReceivesDecals = false;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	SetLevel(1);
	if (BuildingData.IsValidIndex(0))
	{
		if (BuildingData[0].NeedItems.Num() <= 0)
		{
			BuildProgress = 1.0f;
		}
	}
	else
	{
		BuildProgress = 1.0f;
		
	}
	OnRep_BuildProgress();

	if (HasCompleted())
	{
		OnBuildingComplete();
	}
}

void ABuilding::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MainBuildingMesh = BuildingMeshComponent->SkeletalMesh;
}

// Called every frame
void ABuilding::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (HasAuthority())
	{
		if (!HasCompleted() && CanProcessBuild())
		{
			for (auto Wrk : Workers)
			{
				if (Wrk.Worker)
				{
					if (Wrk.Worker->IsProcessBuilding())
					{
						BuildProgress = FMath::Clamp(BuildProgress + GetBuildPercentPerSec(Wrk.Worker) * DeltaTime, 0.0f, 1.0f);
					}
				}
			}
			if(HasCompleted())
				OnBuildingComplete();
		}
	}

}

void ABuilding::SetLevel(int NewLevel)
{
	if (Level != NewLevel)
	{
		Level = NewLevel;

		OnLevelChanged(NewLevel);
		BuildProgress = 0.0f;
		Workers.SetNum(MaxWorkerCount);
	}
}

void ABuilding::OnBuildingComplete_Implementation()
{

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
	DOREPLIFETIME(ABuilding, BuildingData);
	DOREPLIFETIME(ABuilding, Health);
	DOREPLIFETIME(ABuilding, BuildProgress);
}

USkeletalMesh* ABuilding::GetBuildingMesh()
{
	if (BuildingMeshComponent)
	{
		return BuildingMeshComponent->SkeletalMesh;
	}
	return nullptr;
}

FItemCeil ABuilding::GetFirstNeeded()
{
	if (BuildingData.IsValidIndex(Level - 1))
	{
		for (auto Item : BuildingData[Level - 1].NeedItems)
		{
			if (Item.CurrentCount < Item.Count)
			{
				FItemCeil Res;
				Res.ItemClass = Item.ItemClass;
				Res.Count = Item.Count - Item.CurrentCount;
				return Res;
			}
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
		for (auto & W : Workers)
		{
			if (!W.Worker)
			{
				W.Worker = Unit;
				return true;
			}
		}
	}
	return false;
}

void ABuilding::RemoveWorker(ASimpleWorkerUnit* Unit)
{
	int i = Workers.FindLastByPredicate([Unit](const FBuildingWorker & Wrk){
		return Wrk.Worker == Unit;
	});
	if(i != INDEX_NONE)
		Workers[i].Worker = nullptr;
}

bool ABuilding::ContainsWorker(ASimpleWorkerUnit* Worker)
{
	int i = Workers.FindLastByPredicate([Worker](const FBuildingWorker & Wrk) {
		return Wrk.Worker == Worker;
	});
	if (i != INDEX_NONE)
		return true;
	return false;
}

bool ABuilding::CanWorkerAdded() const
{
	for (auto Wrk : Workers)
	{
		if (!Wrk.Worker)
			return true;
	}
	return false;
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
	if (BuildingData.IsValidIndex(Level - 1))
	{
		return BuildingData[Level - 1].GradeGold;
	}
	return 0;
}

int ABuilding::GetRequiredResourceCount(TSubclassOf<UItem> ItemClass) const
{
	if (BuildingData.IsValidIndex(Level - 1))
	{
		for (auto Item : BuildingData[Level - 1].NeedItems)
		{
			if (Item.ItemClass == ItemClass)
				return Item.Count - Item.CurrentCount;
		}
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
	return BuildProgress;
}


void ABuilding::BeginBuild(ASimpleWorkerUnit* Unit)
{
	if (ContainsWorker(Unit))
	{
		Unit->BeginProcessAction(EGameUnitAction::UA_Building, this);
	}
}

void ABuilding::EndBuild(ASimpleWorkerUnit* Unit)
{
	if (ContainsWorker(Unit))
	{
		Unit->EndAction(EGameUnitAction::UA_Building);
	}
}

float ABuilding::GetBuildPercentPerSec(class ASimpleWorkerUnit* Unit) const
{
	if (BuildingData.IsValidIndex(Level - 1))
	{
		return (float) MaxWorkerCount / BuildingData[Level - 1].GradeTime;
	}
	return 1.0f;
}

bool ABuilding::CanProcessBuild() const
{
	if (BuildProgress >= 1.0f) return false;
	if (BuildingData.IsValidIndex(Level - 1))
	{
		for (auto Item : BuildingData[Level - 1].NeedItems)
		{
			if (Item.CurrentCount < Item.Count)
			{
				return false;
			}
		}
		return true;
	}

	return false;
}

int ABuilding::WorkersNum() const
{
	int n = 0;
	for (auto Wrk : Workers)
	{
		if (Wrk.Worker)
			n++;
	}
	return n;
}

void ABuilding::PutResources(FItemCeil NewItem)
{
	if (BuildingData.IsValidIndex(Level - 1))
	{
		for (int i = 0; i < BuildingData[Level - 1].NeedItems.Num(); i++)
		{
			auto & Item = BuildingData[Level - 1].NeedItems[i];
			if (Item.ItemClass == NewItem.ItemClass && Item.CurrentCount < Item.Count)
			{				
				Item.CurrentCount = FMath::Clamp(Item.CurrentCount + NewItem.Count, 0, Item.Count);				
			}
		}
	}
}

void ABuilding::OnRep_BuildProgress()
{
	if (HasCompleted() || ProgressBuildMeshes.Num() == 0)
	{
		BuildingMeshComponent->SetSkeletalMesh(MainBuildingMesh);
	}
	else
	{
		int MeshIndex = BuildProgress * ProgressBuildMeshes.Num();
		if (ProgressBuildMeshes.IsValidIndex(MeshIndex))
		{
			BuildingMeshComponent->SetSkeletalMesh(ProgressBuildMeshes[MeshIndex]);
		}
		else
		{
			BuildingMeshComponent->SetSkeletalMesh(MainBuildingMesh);
		}
	}
}