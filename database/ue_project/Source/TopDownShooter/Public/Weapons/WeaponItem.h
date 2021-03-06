// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gameplay/Item.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API UWeaponItem : public UItem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	USkeletalMesh* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	struct FDamage Damage;
	
};
