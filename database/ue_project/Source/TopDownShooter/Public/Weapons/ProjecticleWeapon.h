// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Public/Weapons/Weapon.h"
#include "ProjecticleWeapon.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TOPDOWNSHOOTER_API AProjecticleWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	
	virtual void Attack(FVector TargetPoint) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Weapon")
	TSubclassOf<class AProjecticle> ProjecticleClass;
};
