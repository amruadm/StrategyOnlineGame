// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "GeneratorBuilding.h"


void AGeneratorBuilding::BeginPlay()
{
	Super::BeginPlay();
}

float AGeneratorBuilding::GetGenerationTime_Implementation() const
{
	return 1.0f;
}