// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StorageInterface.generated.h"

/**
 * 
 */
UINTERFACE(Blueprintable)
class TOPDOWNSHOOTER_API UStorageInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class TOPDOWNSHOOTER_API IStorageInterface
{
	GENERATED_IINTERFACE_BODY()

public:
		
	virtual TArray<FItemCeil> GetItems() const = 0;
	
	virtual int GetStorageSize() const = 0;
	
	/*
		@param TSubclassOf<class UResourceItem> ResourceClass class of find resource
		@return num of finded resources
	*/	
	virtual int ContainsItemOfClass(TSubclassOf<class UItem> ResourceClass) const = 0;

	/* 
		@param UResourceItem ResourceClass class of taken resource
		@param int Count take count
		@return taken items 	
	*/
	virtual FItemCeil TakeItemOfClass(TSubclassOf<class UItem> ResourceClass, int Count) = 0;

	/*
		@return amount of placed resources
	*/
	virtual int PlaceItem(FItemCeil & Item) = 0;

};
