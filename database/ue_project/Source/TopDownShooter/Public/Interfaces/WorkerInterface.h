// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "WorkerInterface.generated.h"

/**
 * 
 */

UINTERFACE(Blueprintable)
class TOPDOWNSHOOTER_API UWorkerInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class TOPDOWNSHOOTER_API IWorkerInterface
{
	GENERATED_IINTERFACE_BODY()

public:

	virtual void PlaceResource(class IStorageInterface* Storage, int index) = 0;

	UFUNCTION(BlueprintNativeEvent, Category = "Worker")
	int GetInventorySize() const;
	
	virtual TArray<FItemCeil> GetItems() const = 0;	

	virtual int GiveResourceFrom(struct FItemCeil Item) = 0;

	/*
		@return num of taked items
	*/
	virtual int TakeItemsOfClass(TSubclassOf<UItem> ItemClass, int Count) = 0;

	virtual bool ContainsItemOfClass(TSubclassOf<UItem> ItemClass, int Count = 0) const = 0;
};