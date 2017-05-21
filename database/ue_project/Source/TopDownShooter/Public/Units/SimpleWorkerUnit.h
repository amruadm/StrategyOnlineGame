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

	ASimpleWorkerUnit();

	virtual void BeginPlay() override;

	virtual void PlaceResource(class IStorageInterface* Storage, int index);

	virtual int GetInventorySize_Implementation() const override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SimpleWorker")
	virtual TArray<FItemCeil> GetItems() const override { return Items; }

	UFUNCTION(BlueprintCallable, Category = "SimpleWorker")
	virtual int GiveResourceFrom(struct FItemCeil Item) override;

	virtual int TakeItemsOfClass(TSubclassOf<UItem> ItemClass, int Count) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleWorker")
	virtual bool ContainsItemOfClass(TSubclassOf<UItem> ItemClass, int Count = 0) const override;

	/*
		return true if worker attached to building
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleWorker")
	bool SetTargetBuilding(ABuilding* NewTarget);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleWorker")
	class ABuilding* GetTargetBuilding() const
	{
		return TargetBuilding;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleWorker")
	bool IsProcessBuilding() const
	{
		return bProcessBuild;
	}
		
protected:

	UPROPERTY(EditDefaultsOnly, Category="SimpleWorker")
	FName AttachmentSocket;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleWorker")
	UStaticMesh* DefaultAttachmentMesh;

	UPROPERTY(Replicated)
	class ABuilding* TargetBuilding;

	UPROPERTY(Replicated)
	bool bProcessBuild;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Items, Category="SimpleWorker")
	TArray<struct FItemCeil> Items;
	
	void ResizeInventory(int NewSize);

	UFUNCTION()
	void OnBeginBuild(EGameUnitAction ActionType);

	UFUNCTION()
	void OnEndBuild(EGameUnitAction ActionType);

private:

	UStaticMeshComponent* Attachment;

	UFUNCTION()
	void OnRep_Items();

};
