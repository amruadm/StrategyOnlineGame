// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnitPlaceInterface.generated.h"

/**
 * 
 */
UINTERFACE(Blueprintable)
class TOPDOWNSHOOTER_API UUnitPlaceInterface: public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class TOPDOWNSHOOTER_API IUnitPlaceInterface
{
	GENERATED_IINTERFACE_BODY()

public:

	virtual void PlaceUnit(class AGameUnit* Unit) = 0;

	virtual void OnUnplace(class AGameUnit* Unit) = 0;

	virtual TArray<class AGameUnit*> GetPlacedUnits() const = 0;

	virtual bool CanBePlaced(class AGameUnit* Unit) const = 0;
};
