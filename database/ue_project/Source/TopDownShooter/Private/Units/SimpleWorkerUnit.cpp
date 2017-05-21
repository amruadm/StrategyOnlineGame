// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Runtime/Engine/Classes/Engine/ActorChannel.h"
#include "Public/Units/SimpleWorkerUnit.h"
#include "Public/Buildings/Building.h"
#include "Public/Resource/ResourceItem.h"

ASimpleWorkerUnit::ASimpleWorkerUnit()
{
	OnBeginProcessAction.AddDynamic(this, &ASimpleWorkerUnit::OnBeginBuild);
	OnEndAction.AddDynamic(this, &ASimpleWorkerUnit::OnEndBuild);

	Attachment = CreateDefaultSubobject<UStaticMeshComponent>("AttachmentMesh");
	Attachment->AttachTo(GetMesh());
}

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
	DOREPLIFETIME(ASimpleWorkerUnit, bProcessBuild);
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
	if (TargetBuilding)
	{
		TargetBuilding->RemoveWorker(this);
	}
	if (NewTarget)
	{
		if (NewTarget->AddWorker(this))
		{
			TargetBuilding = NewTarget;
			return true;
		}
	}
	else
		TargetBuilding = nullptr;
	return false;
}

void ASimpleWorkerUnit::OnBeginBuild(EGameUnitAction ActionType)
{
	if (ActionType == EGameUnitAction::UA_Building)
	{
		bProcessBuild = true;
	}
}

void ASimpleWorkerUnit::OnEndBuild(EGameUnitAction ActionType)
{
	if (ActionType == EGameUnitAction::UA_Building)
	{
		bProcessBuild = false;
	}
}

void ASimpleWorkerUnit::OnRep_Items()
{
	if (!Attachment) return;
	UStaticMesh* TargetMesh = nullptr;
	if (USkeletalMeshComponent* Mesh = GetMesh())
	{
		if (Items.Num() > 1)
		{
			TargetMesh = DefaultAttachmentMesh;
		}
		else if(Items.Num() == 1)
		{
			if (Items[0].ItemClass)
			{
				if (UItem* ItemObject = Items[0].ItemClass->GetDefaultObject<UItem>())
				{
					TargetMesh = ItemObject->AttachmentMesh;
				}
			}
		}
	}
	if (TargetMesh)
	{
		FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, false);
		Attachment->AttachToComponent(GetMesh(), rules, AttachmentSocket);
	}
	Attachment->SetStaticMesh(TargetMesh);
}