// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Public/Weapons/Weapon.h"
#include "MeeleWeapon.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TOPDOWNSHOOTER_API AMeeleWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	virtual void Attack(FVector TargetPoint) override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* damageParticle;
	
};
