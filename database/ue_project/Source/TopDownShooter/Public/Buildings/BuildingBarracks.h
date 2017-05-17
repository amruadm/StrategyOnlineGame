// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Buildings/Building.h"
#include "BuildingBarracks.generated.h"

USTRUCT(BlueprintType)
struct FBarracksItem
{
	GENERATED_BODY()
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AGameUnit> UnitClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FItemCeil> NeededItems;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BuildTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Golds;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int AvaliableLevel = 1;
};

USTRUCT(BlueprintType)
struct FBarracksQueueItemTarget
{
	GENERATED_BODY()
		
	UPROPERTY(BlueprintReadOnly)	
	int TargetIndex;
	
	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<class UItem> ItemClass;
	
	UPROPERTY(BlueprintReadOnly)
	int Count;
};

USTRUCT(BlueprintType)
struct FBarracksQueueItem
{
	GENERATED_BODY()	
	
	UPROPERTY(BlueprintReadOnly)
	int ItemIndex;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FBarracksQueueItemTarget> PushItems;
	
	//UPROPERTY(BlueprintReadOnly)
	//FTimerHandle Timer;
	UPROPERTY(BlueprintReadOnly)
	float ProgressTime = 0.0f;
};

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API ABuildingBarracks : public ABuilding
{
	GENERATED_BODY()
	
public:
	
	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaSeconds ) override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Barracks")
	TArray<FBarracksItem> GetItems() const
	{
		return Items;
	}
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Barracks")
	TArray<FBarracksItem> GetAvaliableItems() const;
	
	virtual void OnBuildingComplete_Implementation() override;
	
	UFUNCTION(Server, Reliable, WithValidation)
	void AddQueueItem(int ItemIndex, const TArray<FBarracksQueueItemTarget> & UsingItems);

	UFUNCTION(Server, Reliable, WithValidation)
	void RemoveQueueItem(int Index);
	
	void PushItem(FItemCeil Item);
	
	FItemCeil GetNeededQueueItem() const;
	
protected:

	UPROPERTY(EditDefaultsOnly, Category="Barracks")
	TArray<FBarracksItem> Items;
	
	UPROPERTY(Replicated)
	TArray<FBarracksQueueItem> Queue;
	
	void SpawnUnit(const FBarracksQueueItem & Item);
	
	UFUNCTION(BlueprintNativeEvent, Category="Barracks")
	FVector GetSpawnPoint() const;
	virtual FVector GetSpawnPoint_Implementation() const;

	bool GetQueuePeekItem(FBarracksQueueItem & PeekItem) const;
	
};
