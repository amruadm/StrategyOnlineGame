// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Buildings/IndustrialBuilding.h"
#include "GeneratorBuilding.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API AGeneratorBuilding : public AIndustrialBuilding
{
	GENERATED_BODY()
	
	
public:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GeneratorBuilding")
	FItemCeil GetOutputItem() const { return OutputItem; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure, Category = "GeneratorBuilding")
	float GetGenerationTime() const;

	UPROPERTY(EditDefaultsOnly, Category = "GeneratorBuilding")
	TSubclassOf<class UItem> OutputItemClass;



protected:

	FItemCeil OutputItem;
	
};
