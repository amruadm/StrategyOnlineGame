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
	
	// Sets default values for this actor's properties
	AGameResource();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "GameResource")
	float GetExtractionTime(class AWorkerUnit* Unit) const;

	UFUNCTION(BlueprintCallable, Category = "GameResource")
	bool CanExtract(class AWorkerUnit* Unit) const;

	UFUNCTION(BlueprintCallable, Category = "GameResource")
	int GetExtractionCount(class AWorkerUnit* Unit) const;

	UFUNCTION(BlueprintCallable, Category="GameResource")
	void BeginExtract(class AWorkerUnit* Unit);

	UFUNCTION(BlueprintCallable, Category = "GameResource")
	void ProcessExtract(class AWorkerUnit* Unit);

	void DieResource();
	
protected:

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameResource")
	int Count;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameResource")
	int MinGenerateCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameResource")
	int MaxGenerateCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameResource")
	EGameUnitAction ProcessAnimType;
	
};
