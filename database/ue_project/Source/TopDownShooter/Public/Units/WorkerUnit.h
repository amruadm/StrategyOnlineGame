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

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Worker")
	int GetEfficiency() const;

	UFUNCTION(BlueprintCallable, Category = "Worker")
	void SetOwnExtractor(class AIndustrialBuilding* Extractor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Worker")
	AIndustrialBuilding* GetOwnExtractor() const { return OwnExtractor; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Worker")
	int GetWorkplaceIndex() const
	{
		return WorkplaceIndex;
	}

	void SetWorkplaceIndex(int index)
	{
		WorkplaceIndex = index;
	}

	UFUNCTION(BlueprintCallable, Category = "Worker")
	void BeginExtract();

	UFUNCTION(BlueprintCallable, Category = "Worker")
	void EndExtract();

	void EquipTool(TSubclassOf<class UWorkerToolItem> NewTool);

	void EquipDefaultTool();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Worker")
	FName WeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Worker")
	TSubclassOf<class AGameResource> ResourceClass;

protected:

	UPROPERTY(Replicated)
	class AIndustrialBuilding* OwnExtractor;

	UPROPERTY(Replicated)
	class AGameResource* TargetResource;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Worker")
	TSubclassOf<class UWorkerToolItem> DefaultToolClass;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentTool, Category="Worker")
	TSubclassOf<class UWorkerToolItem> CurrentTool;

	UPROPERTY(Replicated)
	int WorkplaceIndex;

private:

	UStaticMeshComponent* WeaponAttachmentComponent;

	UFUNCTION()
	void OnRep_CurrentTool();

};
