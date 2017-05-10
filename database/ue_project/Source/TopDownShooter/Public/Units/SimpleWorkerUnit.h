// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Units/CitizenUnit.h"
#include "SimpleWorkerUnit.generated.h"
/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API ASimpleWorkerUnit : public ACitizenUnit, public IWorkerInterface
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	virtual void PlaceResource(class IStorageInterface* Storage, int index);

	virtual int GetInventorySize_Implementation() const override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SimpleWorker")
	virtual TArray<FItemCeil> GetItems() const override { return Items; }

	virtual int GiveResourceFrom(struct FItemCeil & Item, int Count) override;

	virtual int TakeItemsOfClass(TSubclassOf<UItem> ItemClass, int Count) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleWorker")
	virtual bool ContainsItemOfClass(TSubclassOf<UItem> ItemClass, int Count = 0) const override;
		
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category="SimpleWorker")
	TArray<struct FItemCeil> Items;
	
	void ResizeInventory(int NewSize);

};
