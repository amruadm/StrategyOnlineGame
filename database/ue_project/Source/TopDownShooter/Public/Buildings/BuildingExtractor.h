// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Buildings/IndustrialBuilding.h"
#include "BuildingExtractor.generated.h"

UENUM(BlueprintType)
enum class EExtractorMode : uint8
{
	EM_Stop UMETA(DisplayName = "Stop working"),
	EM_Loop UMETA(DisplayName = "Loop process"),
	EM_Fixed UMETA(DisplayName = "Fixed product count")
};

USTRUCT(BlueprintType)
struct FExtractorWorkplace: public FBuildingWorkplace
{
	GENERATED_BODY()

	UPROPERTY()
	TEnumAsByte<EExtractorMode> WorkMode;

	UPROPERTY()
	TArray<int> ItemIndices;
		
	UPROPERTY()
	FTimerHandle Timer;
};

USTRUCT(BlueprintType)
struct FExtractorItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float MinExp;

	UPROPERTY(EditDefaultsOnly)
	float Timing;

	UPROPERTY(EditDefaultsOnly)
	TArray<FItemCeil> Recipe;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItem> OutputItem;
};

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API ABuildingExtractor : public AIndustrialBuilding
{
	GENERATED_BODY()
	
	
public:
				
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Extractor")
	TArray<FExtractorItem> Items;
		
	UFUNCTION(BlueprintCallable, Category = "Building")
	void BeginConversion(class AWorkerUnit* Unit);

	UFUNCTION(BlueprintCallable, Category = "Building")
	void ProcessConvertion(class AWorkerUnit* Unit);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Building")
	float GetTiming(AWorkerUnit* Worker) const;

	virtual void AddWorker(class AWorkerUnit* Unit) override;

	//--------IUnitPlaceInterface implementation

	virtual void PlaceUnit(class AGameUnit* Unit) override;

	virtual void OnUnplace(class AGameUnit* Unit) override;

	virtual TArray<class AGameUnit*> GetPlacedUnits() const override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Buildibg")
	virtual bool CanBePlaced(class AGameUnit* Unit) const override;

	//----------------------------------------------
	
	virtual int GetWorkplacesNum_Implementation() const override;

protected:

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Extractor")
	TArray<FExtractorWorkplace> Workplaces;

	
	
};
