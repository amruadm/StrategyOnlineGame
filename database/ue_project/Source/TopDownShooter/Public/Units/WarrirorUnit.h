// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Units/CitizenUnit.h"
#include "WarrirorUnit.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API AWarrirorUnit : public ACitizenUnit
{
	GENERATED_BODY()
	
public:

	virtual void SetTargetPoint_Implementation(const struct FCommandTarget & Target) override;
	virtual bool CanBeMultipleSelected_Implementation() override { return true; }
	
};
