// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/Item.h"
#include "ResourceItem.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UResourceItem : public UItem
{
	GENERATED_BODY()
	
public:
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Resource")
	//TSubclassOf<class AGameResource> ResourceClass;

	// Need to make a placeable resource or all placeble item objects, for drop items from storage, or inventory
	
};
