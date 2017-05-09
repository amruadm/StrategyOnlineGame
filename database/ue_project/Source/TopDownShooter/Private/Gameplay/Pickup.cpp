// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Gameplay/Pickup.h"


// Sets default values
APickup::APickup()
{
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerCollider"));

	// Both colliders need to have this set to true for events to fire
	Trigger->bGenerateOverlapEvents = true;

	// Set the collision mode for the collider
	// This mode will only enable the collider for raycasts, sweeps, and overlaps
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

bool APickup::CanBePickup(class ATopDownShooterCharacter* pawn) const
{
	return true;
}

void APickup::NotifyActorBeginOverlap(AActor* Other)
{

}

void APickup::NotifyActorEndOverlap(AActor* Other)
{

}