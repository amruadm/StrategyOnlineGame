// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameHelpers.generated.h"

UCLASS()
class TOPDOWNSHOOTER_API UGameHelpers: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Game")
	static UObject* SpawnObjectFromClass(UClass* ObjectClass);

	/** find intersection of ray in world space with ground plane */
	static FVector IntersectRayWithPlane(const FVector& RayOrigin, const FVector& RayDirection, const FPlane& Plane);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Game")
	static bool AsTeam(TScriptInterface<class ITeamObjectInterface> ObjectA, TScriptInterface<class ITeamObjectInterface> ObjectB);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Game")
	static bool UnitsAsTeam(class AGameUnit* ObjectA, class AGameUnit* ObjectB);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Game")
	static class ABuilding* GetNearestBuilding(class ASimpleWorkerUnit* Unit);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Game")
	static int ContainsResourceAtStorage(TScriptInterface<class IStorageInterface> Storage, TSubclassOf<class UResourceItem> ResourceClass);

	UFUNCTION(BlueprintCallable, Category = "Game")
	static void UnitPlaceTo(class AGameUnit* Unit, TScriptInterface<class IUnitPlaceInterface> Target);

	/*
		----------------Worker interface Blueprint method implementation
	*/

	UFUNCTION(BlueprintCallable, Category = "Worker")
	static void PlaceResource(TScriptInterface<class IWorkerInterface> Target, TScriptInterface<class IStorageInterface> Storage, int index);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Worker")
	static int GetInventorySize(TScriptInterface<class IWorkerInterface> Target);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Worker")
	static TArray<struct FItemCeil> GetItems(TScriptInterface<class IWorkerInterface> Target);

	UFUNCTION(BlueprintCallable, Category = "Worker")
	static int GiveResourceFrom(TScriptInterface<class IWorkerInterface> Target, UPARAM(ref) FItemCeil & Item, int Count);

	UFUNCTION(BlueprintCallable, Category = "Worker")
	static int TakeItemsOfClass(TScriptInterface<class IWorkerInterface> Target, TSubclassOf<UItem> ItemClass, int Count);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Worker")
	static bool ContainsItemOfClass(TScriptInterface<class IWorkerInterface> Target, TSubclassOf<UItem> ItemClass, int Count = 0);
		
};