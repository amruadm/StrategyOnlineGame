// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GameResource.generated.h"

UCLASS(Blueprintable)
class TOPDOWNSHOOTER_API AGameResource : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameResource")
	TSubclassOf<class UResourceItem> ItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameResource")
	int MinExp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameResource")
	EGameUnitAction ProcessAnimType;
	
	// Sets default values for this actor's properties
	AGameResource();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure, Category = "GameResource")
	float GetExtractionTime(class AWorkerUnit* Unit) const;
	virtual float GetExtractionTime_Implementation(class AWorkerUnit* Unit) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameResource")
	bool CanExtract(class AWorkerUnit* Unit) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure, Category = "GameResource")
	int GetExtractionCount(class AWorkerUnit* Unit) const;
	virtual int GetExtractionCount_Implementation(class AWorkerUnit* Unit) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure, Category = "GameResource")
	int GetExtractionExp(class AWorkerUnit* Unit) const;
	virtual int GetExtractionExp_Implementation(class AWorkerUnit* Unit) const;

	FItemCeil Extract(class AWorkerUnit* Unit);

	void DieResource();
	
protected:

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameResource")
	int Count;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameResource")
	int MinGenerateCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameResource")
	int MaxGenerateCount;
	
};
