// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Weapons/WeaponPickup.h"
#include "TopDownShooterCharacter.h"




void AWeaponPickup::NotifyActorBeginOverlap(AActor* Other)
{
	/*ATopDownShooterCharacter* pawn = Cast<ATopDownShooterCharacter>(Other);
	if (pawn && CanBePickup(pawn))
	{
		pawn->TakeWeapon((AWeapon*)(WeaponClass->GetDefaultObject()), Ammo);
	}*/
}

bool AWeaponPickup::CanBePickup(class ATopDownShooterCharacter* pawn) const
{
	return WeaponClass->IsInA(pawn->MainWeaponClass);
}