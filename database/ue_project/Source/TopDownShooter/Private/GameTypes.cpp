// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Resource/ResourceItem.h"

FItemCeil FItemCeil::EmptyItem = FItemCeil(0, nullptr);

int FItemCeil::GetMaxStack() const
{
	if (!ItemClass) return 0;
	UItem* item = ItemClass->GetDefaultObject<UItem>();
	if (item)
	{
		return item->MaxCount;
	}
	return 0;
}

FDamage::FDamage()
{
	Random.Initialize(FMath::RandRange(0, 1000));
}

float FDamage::GetDamage() const
{
	float Dmg = Random.FRandRange(Min, Max);
	return Dmg;
}