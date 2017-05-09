// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Public/Gameplay/Pickup.h"
#include "Public/Weapons/Weapon.h"
#include "WeaponPickup.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TOPDOWNSHOOTER_API AWeaponPickup : public APickup
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Pickup")
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	int Ammo;
	
	virtual void NotifyActorBeginOverlap(AActor* Other) override;
	virtual bool CanBePickup(class ATopDownShooterCharacter* pawn) const;
};
