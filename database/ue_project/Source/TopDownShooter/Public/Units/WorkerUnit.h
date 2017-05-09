// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Units/SimpleWorkerUnit.h"
#include "WorkerUnit.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API AWorkerUnit : public ASimpleWorkerUnit
{
	GENERATED_BODY()
	
public:

	AWorkerUnit();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Worker")
	int GetEfficiency() const;

	UFUNCTION(BlueprintCallable, Category = "Worker")
	void SetOwnExtractor(class ABuildingExtractor* Extractor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Worker")
	ABuildingExtractor* GetOwnExtractor() const { return OwnExtractor; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Worker")
	int GetWorkplaceIndex() const
	{
		return WorkplaceIndex;
	}

	void SetWorkplaceIndex(int index)
	{
		WorkplaceIndex = index;
	}

	void EquipTool(class AToolWeapon* NewTool);

	void EquipDefaultTool();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Worker")
	FName WeaponSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Worker")
	TSubclassOf<class AGameResource> ResourceClass;

protected:

	UPROPERTY(Replicated)
	class ABuildingExtractor* OwnExtractor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Worker")
	TSubclassOf<class AToolWeapon> DefaultToolClass;

	UPROPERTY(BlueprintReadOnly, Category="Worker")
	class AToolWeapon* CurrentTool;

	UPROPERTY(Replicated)
	int WorkplaceIndex;

};
