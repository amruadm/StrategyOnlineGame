// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Units/SimpleWorkerUnit.h"
#include "Public/Buildings/Building.h"
#include "Public/Resource/ResourceItem.h"

UObject* UGameHelpers::SpawnObjectFromClass(UClass* ObjectClass)
{
	return NewObject<UObject>(GetTransientPackage(), ObjectClass);
}

FControlData UGameHelpers::GetUIData(TScriptInterface<ISelectable> Selectable)
{
	FControlData Result;
	if (Selectable)
	{
		Result = Selectable->Execute_GetUIData(Selectable.GetObject());
	}
	return Result;
}

FVector UGameHelpers::IntersectRayWithPlane(const FVector& RayOrigin, const FVector& RayDirection, const FPlane& Plane)
{
	const FVector PlaneNormal = FVector(Plane.X, Plane.Y, Plane.Z);
	const FVector PlaneOrigin = PlaneNormal * Plane.W;

	const float Distance = FVector::DotProduct((PlaneOrigin - RayOrigin), PlaneNormal) / FVector::DotProduct(RayDirection, PlaneNormal);
	return RayOrigin + RayDirection * Distance;
}

bool UGameHelpers::AsTeam(TScriptInterface<ITeamObjectInterface> ObjectA, TScriptInterface<ITeamObjectInterface> ObjectB)
{
	return ObjectA->GetTeamNum() == ObjectB->GetTeamNum();
}

bool UGameHelpers::UnitsAsTeam(AGameUnit* ObjectA, AGameUnit* ObjectB)
{
	return ObjectA->GetTeamNum() == ObjectB->GetTeamNum();
}

ABuilding* UGameHelpers::GetNearestBuilding(ASimpleWorkerUnit* Unit)
{
	if (Unit)
	{
		float lastDist = -1.0f;
		ABuilding* result = nullptr;
		for (TActorIterator<ABuilding> ActorItr(Unit->GetWorld()); ActorItr; ++ActorItr)
		{
			float d = FVector::DistSquared(ActorItr->GetActorLocation(), Unit->GetActorLocation());
			if (ActorItr->GetTeamNum() == Unit->GetTeamNum()
				&& (d < lastDist || lastDist < 0.0f) && !ActorItr->HasCompleted() && ActorItr->CanWorkerAdded())
			{
				lastDist = FVector::DistSquared(ActorItr->GetActorLocation(), Unit->GetActorLocation());
				result = *ActorItr;
			}
		}
		return result;
	}
	return nullptr;
}

int UGameHelpers::ContainsResourceAtStorage(TScriptInterface<IStorageInterface> Storage, TSubclassOf<UResourceItem> ResourceClass)
{
	return Storage->ContainsItemOfClass(ResourceClass);
}

void UGameHelpers::UnitPlaceTo(AGameUnit* Unit, TScriptInterface<IUnitPlaceInterface> Target)
{
	if(Unit && Target)
		Target->PlaceUnit(Unit);
}

/*
----------------Worker interface Blueprint method implementation
*/

void UGameHelpers::PlaceResource(TScriptInterface<IWorkerInterface> Target, TScriptInterface<class IStorageInterface> Storage, int index)
{
	Target->PlaceResource(Cast<IStorageInterface>(Storage.GetObject()), index);
}

int UGameHelpers::GetInventorySize(TScriptInterface<IWorkerInterface> Target)
{
	return Target->GetInventorySize();
}

TArray<FItemCeil> UGameHelpers::GetItems(TScriptInterface<IWorkerInterface> Target)
{
	return Target->GetItems();
}

int UGameHelpers::GiveResourceFrom(TScriptInterface<IWorkerInterface> Target, UPARAM(ref) FItemCeil & Item)
{
	return Target->GiveResourceFrom(Item);
}

int UGameHelpers::TakeItemsOfClass(TScriptInterface<IWorkerInterface> Target, TSubclassOf<UItem> ItemClass, int Count)
{
	return Target->TakeItemsOfClass(ItemClass, Count);
}

bool UGameHelpers::ContainsItemOfClass(TScriptInterface<IWorkerInterface> Target, TSubclassOf<UItem> ItemClass, int Count)
{
	return Target->ContainsItemOfClass(ItemClass, Count);
}

//----------------------------------------------

UTexture2D* UGameHelpers::GetItemIcon(TSubclassOf<UItem> Target)
{
	if (Target)
	{
		if (UItem* ItemObject = Target->GetDefaultObject<UItem>())
		{
			return ItemObject->Icon;
		}
	}
	return nullptr;
}

UTexture2D* UGameHelpers::GetBuildingIcon(TSubclassOf<ABuilding> Target)
{
	if (Target)
	{
		if (ABuilding* BuildingObject = Target->GetDefaultObject<ABuilding>())
		{
			return BuildingObject->Icon;
		}
	}
	return nullptr;
}