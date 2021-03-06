// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Units/GameUnit.h"
#include "Public/Buildings/BuildingBarracks.h"


void ABuildingBarracks::BeginPlay()
{
  Super::BeginPlay();
}

void ABuildingBarracks::Tick( float DeltaSeconds )
{
  Super::Tick(DeltaSeconds);
  if (HasAuthority())
  {
	  FBarracksQueueItem PeekItem;
	  if (GetQueuePeekItem(PeekItem) && IsPeekItemFull())
	  {
		  if (!Items.IsValidIndex(PeekItem.ItemIndex))
			  if (PeekItem.ProgressTime >= Items[PeekItem.ItemIndex].BuildTime)
			  {
				  SpawnUnit(PeekItem);
				  Queue.RemoveAt(Queue.Num() - 1);
			  }
			  else
				  PeekItem.ProgressTime += DeltaSeconds;
	  }
  }
}

void ABuildingBarracks::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
  DOREPLIFETIME_CONDITION(ABuildingBarracks, Queue, COND_OwnerOnly);
}

TArray<FBarracksItem> ABuildingBarracks::GetAvaliableItems() const
{
  TArray<FBarracksItem> Result;
  for(auto Item : Items)
  {
	  if (Item.AvaliableLevel >= Level)
		  Result.Add(Item);
  }
  return Result;
}

void ABuildingBarracks::OnBuildingComplete_Implementation()
{
	Super::OnBuildingComplete_Implementation();
}

void ABuildingBarracks::AddQueueItem_Implementation(int ItemIndex, const TArray<FBarracksQueueItemTarget> & UsingItems)
{
  if(!Items.IsValidIndex(ItemIndex)) return;
  const FBarracksItem & BarrackItem = Items[ItemIndex];
  
  FBarracksQueueItem NewItem;
  NewItem.ItemIndex = ItemIndex;
  for(auto Item : UsingItems)
  {
    if(!BarrackItem.NeededItems.IsValidIndex(Item.TargetIndex))
      return;
    if(!Item.ItemClass->IsChildOf(BarrackItem.NeededItems[Item.TargetIndex].ItemClass))
      return;
    NewItem.PushItems.Add(Item);
  }
  Queue.Add(NewItem);
}

bool ABuildingBarracks::AddQueueItem_Validate(int ItemIndex, const TArray<FBarracksQueueItemTarget> & UsingItems)
{
  return true;
}

void ABuildingBarracks::RemoveQueueItem_Implementation(int Index)
{
	
}

bool ABuildingBarracks::RemoveQueueItem_Validate(int Index)
{
	return true;
}

void ABuildingBarracks::PushItem(FItemCeil Item)
{
	FBarracksQueueItem PeekItem;
	if(GetQueuePeekItem(PeekItem))
	{
		FBarracksItem BarrackItem = Items[PeekItem.ItemIndex];
		for(FBarracksQueueItemTarget & QueueItem : PeekItem.PushItems)
		{	
			if(Item.ItemClass == QueueItem.ItemClass
				&& QueueItem.Count < BarrackItem.NeededItems[QueueItem.TargetIndex].Count)
			{
				QueueItem.Count = FMath::Clamp(QueueItem.Count + Item.Count, 0, BarrackItem.NeededItems[QueueItem.TargetIndex].Count);
			}
		}
	}
}

FItemCeil ABuildingBarracks::GetNeededQueueItem() const
{
	FBarracksQueueItem PeekItem;
	if(GetQueuePeekItem(PeekItem))
	{
		FBarracksItem BarrackItem = Items[PeekItem.ItemIndex];
		for(FBarracksQueueItemTarget & QueueItem : PeekItem.PushItems)
		{
			if(QueueItem.Count < BarrackItem.NeededItems[QueueItem.TargetIndex].Count)
			{
				FItemCeil Result;
				Result.ItemClass = QueueItem.ItemClass;
				Result.Count = QueueItem.Count;
				return Result;
			}
		}
	}
	FItemCeil ZeroResult;
	return ZeroResult;
}

void ABuildingBarracks::SpawnUnit(const FBarracksQueueItem & Item)
{
	const FBarracksItem & BarracksItem = Items[Item.ItemIndex];
	if(AGameUnit* Unit = GetWorld()->SpawnActor<AGameUnit>(BarracksItem.UnitClass, GetSpawnPoint(), FRotator::ZeroRotator))
	{
		Unit->SetTeamNum(GetTeamNum());
	}
}

FVector ABuildingBarracks::GetSpawnPoint_Implementation() const
{
	return GetActorLocation();
}

bool ABuildingBarracks::GetQueuePeekItem(FBarracksQueueItem & PeekItem) const
{
	if (!Queue.IsValidIndex(Queue.Num() - 1)) return false;
	PeekItem = Queue[Queue.Num() - 1];
	return false;
}

bool ABuildingBarracks::IsPeekItemFull() const
{
	FBarracksQueueItem PeekItem;
	if (GetQueuePeekItem(PeekItem))
	{
		FBarracksItem BarrackItem = Items[PeekItem.ItemIndex];
		for (FBarracksQueueItemTarget Item : PeekItem.PushItems)
		{
			if (Item.Count < BarrackItem.NeededItems[Item.TargetIndex].Count)
			{
				return false;
			}
		}
		return true;
	}
	return false;
}