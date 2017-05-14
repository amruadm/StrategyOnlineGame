// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Runtime/Engine/Classes/Engine/ActorChannel.h"
#include "Public/Units/SimpleWorkerUnit.h"
#include "Public/Buildings/Building.h"
#include "Public/Resource/ResourceItem.h"

void ASimpleWorkerUnit::BeginPlay()
{
	Super::BeginPlay();

	ResizeInventory(1);
}

void ASimpleWorkerUnit::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASimpleWorkerUnit, Items);
	DOREPLIFETIME(ASimpleWorkerUnit, TargetBuilding);
}

void ASimpleWorkerUnit::PlaceResource(IStorageInterface* Storage, int Index)
{
	if (!Items.IsValidIndex(Index)) return;
	Storage->PlaceItem(Items[Index]);
}

int ASimpleWorkerUnit::GetInventorySize_Implementation() const
{
	return 1;
}

int ASimpleWorkerUnit::GiveResourceFrom(FItemCeil Item)
{
	int cnt = Item.Count;
	for (FItemCeil & item : Items)
	{
		if (item.ItemClass == Item.ItemClass || !item.ItemClass)
		{
			cnt = Item.TakeTo(item, cnt);
			if (cnt <= 0) continue;
		}
	}
	return cnt;
}

int ASimpleWorkerUnit::TakeItemsOfClass(TSubclassOf<UItem> ItemClass, int Count)
{
	int cnt = Count;
	for (FItemCeil & item : Items)
	{
		if (cnt <= 0) return Count;
		if (item.ItemClass == ItemClass)
		{
			if (item.Count > 0)
			{
				if (item.Count > cnt)
				{
					item.Count -= cnt;
					cnt = 0;
				}
				else
				{
					cnt -= item.Count;
					item.Count = 0;
					item.ItemClass = nullptr;
				}
			}
		}
	}
	return Count - cnt;
}

bool ASimpleWorkerUnit::ContainsItemOfClass(TSubclassOf<UItem> ItemClass, int Count) const
{
	if (Count <= 0)
	{
		return Items.ContainsByPredicate([ItemClass](const FItemCeil & item) {
			return item.ItemClass == ItemClass;
		});
	}
	else
	{
		int cnt = 0;
		for (const FItemCeil & item : Items)
		{
			if (item.ItemClass == ItemClass)
			{
				cnt += item.Count;
				if (cnt >= Count) return true;
			}
		}
		return cnt >= Count;
	}
}

void ASimpleWorkerUnit::ResizeInventory(int NewSize)
{
	Items.SetNum(NewSize);
}

bool ASimpleWorkerUnit::SetTargetBuilding(ABuilding* NewTarget)
{
	if (!NewTarget) return false;
	if (TargetBuilding)
	{
		TargetBuilding->RemoveWorker(this);
	}
	if (NewTarget->AddWorker(this))
	{
		TargetBuilding = NewTarget;
		return true;
	}
	return false;
}