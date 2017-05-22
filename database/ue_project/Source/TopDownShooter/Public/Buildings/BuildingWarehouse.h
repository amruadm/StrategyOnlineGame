// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Buildings/Building.h"
#include "Public/Interfaces/StorageInterface.h"
#include "BuildingWarehouse.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API ABuildingWarehouse : public ABuilding, public IStorageInterface
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	virtual TArray<FItemCeil> GetItems() const override;

	virtual int GetStorageSize() const override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Warehouse")
	virtual int ContainsItemOfClass(TSubclassOf<class UItem> ResourceClass) const override;

	UFUNCTION(BlueprintCallable, Category = "Warehouse")
	virtual FItemCeil TakeItemOfClass(TSubclassOf<class UItem> ResourceClass, int Count) override;

	UFUNCTION(BlueprintCallable, Category="Warehouse")
	virtual int PlaceItem(FItemCeil & Item) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure, Category = "Warehouse")
	bool IsTakingOnly() const;
	virtual bool IsTakingOnly_Implementation() const
	{
		return false;
	}
	

	virtual void OnLevelChanged_Implementation(int NewLevel) override;

	virtual FControlData GetUIData_Implementation() override;
	
protected:

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Warehouse")
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Warehouse")
	TArray<FItemCeil> Resources;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, Category = "Warehouse")
	int StorageSize;
	
	void ResizeStorage();
};
