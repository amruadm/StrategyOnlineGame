// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Buildings/IndustrialBuilding.h"
#include "ResourceBuilding.generated.h"

USTRUCT(BlueprintType)
struct FResourceWorkplace: public FBuildingWorkplace
{
	GENERATED_BODY()

};


/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API AResourceBuilding : public AIndustrialBuilding
{
	GENERATED_BODY()
	
	
public:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ResourceBuilding")
	TSubclassOf<class UItem> InputResourceItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ResourceBuilding")
	TSubclassOf<class AGameResource> ResourceClass;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ResourceBuilding")
	TArray<FResourceWorkplace> GetWorkplaces() const { return Workplaces; };

	virtual void AddWorker(class AWorkerUnit* Unit) override;
		
	virtual int GetWorkplacesNum_Implementation() const override;

	//--------IUnitPlaceInterface implementation

	virtual void PlaceUnit(class AGameUnit* Unit) override;

	virtual void OnUnplace(class AGameUnit* Unit) override;

	virtual TArray<class AGameUnit*> GetPlacedUnits() const override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Buildibg")
	virtual bool CanBePlaced(class AGameUnit* Unit) const override;

	//----------------------------------------------

protected:

	TArray<FResourceWorkplace> Workplaces;

	virtual void ResizeWorkplaces() override;
	
};
