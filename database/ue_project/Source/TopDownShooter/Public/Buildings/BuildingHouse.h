// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Buildings/Building.h"
#include "BuildingHouse.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API ABuildingHouse : public ABuilding
{
	GENERATED_BODY()
	
public:

	ABuildingHouse();

	virtual void BeginPlay() override;

	void SpawnCitizen();

	UPROPERTY(Replicated)
	TArray<class ACitizenUnit*> Citizens;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BuildingHouse")
	TSubclassOf<ACitizenUnit> CitizenClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BuildingHouse")
	float SpawnTimeInterval;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "BuildingHouse")
	int MaxCitizens;

	virtual void OnBuildingComplete_Implementation() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BuildingHouse")
	UVectorFieldComponent* SpawnPoint;

	FTimerHandle SpawnTimer;
	
};
