// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Weapons/ProjecticleWeapon.h"
#include "Public/Weapons/Projecticle.h"
#include "TopDownShooterCharacter.h"
#include "UnrealNetwork.h"

void AProjecticleWeapon::Attack(FVector TargetPoint)
{
	if (!Owner)
	{
		return;
	}

	AProjecticle* projecticle = GetWorld()->SpawnActor<AProjecticle>(ProjecticleClass);
	if (projecticle)
	{
		FVector spawnPoint = Owner->GetActorLocation() + Owner->GetActorRotation().Vector() * 30;
		FVector loc = spawnPoint;
		loc.Z = 0.0f;
		TargetPoint.Z = 0.0f;
		FVector dir = TargetPoint - loc;
		dir.Normalize();
		projecticle->SetActorLocation(spawnPoint);
		projecticle->SetupProjecticle(dir, this);
	}
}

void AProjecticleWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AProjecticleWeapon, ProjecticleClass);
}