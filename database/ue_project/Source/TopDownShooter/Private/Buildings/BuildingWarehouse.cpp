// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Units/SimpleWorkerUnit.h"
#include "Public/Buildings/BuildingWarehouse.h"
#include "Public/Resource/ResourceItem.h"

void ABuildingWarehouse::BeginPlay()
{
	Super::BeginPlay();

	Resources.Empty();
	Resources.AddDefaulted(GetStorageSize());
}

TArray<FItemCeil> ABuildingWarehouse::GetItems() const
{
	return Resources;
}

int ABuildingWarehouse::GetStorageSize() const
{
	return StorageSize + Level * 2;
}

int ABuildingWarehouse::ContainsItemOfClass(TSubclassOf<UItem> ResourceClass) const
{
	int i = Resources.FindLastByPredicate([ResourceClass](const FItemCeil & item) {
		return item.ItemClass == ResourceClass;
	});
	if (i > -1) 
		return Resources[i].Count;
	else 
		return 0;
}

FItemCeil ABuildingWarehouse::TakeItemOfClass(TSubclassOf<class UItem> ResourceClass, int Count)
{
	if (Count > 0)
	{
		FItemCeil resultItem = FItemCeil::EmptyItem;
		int needed = Count;
		for (FItemCeil & item : Resources)
		{
			if (item.ItemClass == ResourceClass)
			{
				needed = item.TakeTo(resultItem, needed);
			}
		}
		return resultItem;
	}
	return FItemCeil::EmptyItem;
}

int ABuildingWarehouse::PlaceItem(FItemCeil & Item)
{
	int placed = 0;
	for (FItemCeil & item : Resources)
	{
		if (item.ItemClass == Item.ItemClass)
		{
			Item.TakeTo(item, Item.Count);
			if (Item.HasEmpty()) break;
		}		
	}
	return placed;
}

void ABuildingWarehouse::OnLevelChanged_Implementation(int NewLevel)
{

}