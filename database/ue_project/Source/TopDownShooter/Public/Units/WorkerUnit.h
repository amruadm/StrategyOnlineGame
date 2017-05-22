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

	virtual void Tick(float DeltaSeconds) override;

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
	void BeginExtract(class AGameResource* TargetResource);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Worker")
	TSubclassOf<class UWorkerToolItem> DefaultToolClass;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentTool, Category="Worker")
	TSubclassOf<class UWorkerToolItem> CurrentTool;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Worker")
	FName WeaponAttachmentSocket;

	UPROPERTY(Replicated)
	int WorkplaceIndex;

private:

	UStaticMeshComponent* WeaponAttachmentComponent;

	EGameUnitAction ResourceActionType;

	FTimerHandle ActionTimer;

	UFUNCTION()
	void OnRep_CurrentTool();

};
