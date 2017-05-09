// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Buildings/Building.h"
#include "IndustrialBuilding.generated.h"

USTRUCT(BlueprintType)
struct FBuildingWorkplace
{
	GENERATED_BODY()

	UPROPERTY()
	class AWorkerUnit* Worker;
};

/**
 * 
 */
UCLASS(abstract)
class TOPDOWNSHOOTER_API AIndustrialBuilding : public ABuilding
{
	GENERATED_BODY()
	
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Extractor")
	TSubclassOf<class AWorkerUnit> WorkerClass;

	virtual void AddWorker(class AWorkerUnit* Unit) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Building")
	int GetWorkplacesNum() const;
	
};
