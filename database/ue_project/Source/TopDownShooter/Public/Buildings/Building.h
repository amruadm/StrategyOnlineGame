// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Public/GameTypes.h"
#include "Public/Interfaces/TeamObjectInterface.h"
#include "Public/Interfaces/Selectable.h"
#include "Building.generated.h"

USTRUCT(BlueprintType)
struct FBuildingItemCeil : public FItemCeil
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int CurrentCount = 0;
};

USTRUCT(BlueprintType)
struct FBuildingWorker
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	class ASimpleWorkerUnit* Worker = nullptr;
};

USTRUCT(BlueprintType)
struct FBuildingLevelData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GradeGold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GradeTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FBuildingItemCeil> NeedItems;
};

UCLASS(Blueprintable)
class TOPDOWNSHOOTER_API ABuilding : public AActor, public ITeamObjectInterface, public IUnitPlaceInterface, public ISelectable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	USkeletalMeshComponent* GetMeshComponent() const
	{
		return BuildingMeshComponent;
	}

	virtual int GetTeamNum() const override { return TeamNum; }

	virtual void SetTeamNum(int number) override { TeamNum = number; }

	void SetLevel(int NewLevel);

	int GetLevel() const { return Level; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Building")
	int GetMaxLevel() const { return MaxLevel; };

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Building")
	void OnLevelChanged(int NewLevel);

	UFUNCTION(BlueprintCallable, Category = "Buildibg")
	void BeginBuild(class ASimpleWorkerUnit* Unit);

	UFUNCTION(BlueprintCallable, Category = "Buildibg")
	void EndBuild(class ASimpleWorkerUnit* Unit);

	UFUNCTION(BlueprintCallable, Category = "Buildibg")
	void PutResources(FItemCeil NewItem);

	UFUNCTION(BlueprintCallable, Category = "Buildibg")
	bool HasCompleted() const;

	UFUNCTION(BlueprintCallable, Category = "Buildibg")
	bool AddWorker(class ASimpleWorkerUnit* Unit);

	UFUNCTION(BlueprintCallable, Category = "Buildibg")
	void RemoveWorker(class ASimpleWorkerUnit* Unit);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Buildibg")
	bool ContainsWorker(class ASimpleWorkerUnit* Unit);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Buildibg")
	bool CanWorkerAdded() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Buildibg")
	int WorkersNum() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Buildibg")
	FItemCeil GetFirstNeeded();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Buildibg")
	float GetBuildProgress() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Buildibg")
	float GetBuildPercentPerSec(class ASimpleWorkerUnit* Unit) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Buildibg")
	bool CanProcessBuild() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Buildibg")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Buildibg")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Buildibg")
	void Upgrade();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintPure, Category = "Buildibg")
	int GetGradeCost() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Buildibg")
	void OnBuildingComplete();
	virtual void OnBuildingComplete_Implementation();

	virtual void PlaceUnit(class AGameUnit* Unit) override;

	virtual void OnUnplace(class AGameUnit* Unit) override {};

	virtual TArray<class AGameUnit*> GetPlacedUnits() const override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Buildibg")
	virtual bool CanBePlaced(class AGameUnit* Unit) const override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Buildibg")
	int GetRequiredResourceCount(TSubclassOf<class UItem> ItemClass) const;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building")
	int MaxWorkerCount;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building")
	UTexture2D* Icon;

	USkeletalMesh* GetBuildingMesh();

	/*ISelectable implementation*/
	virtual void Selected_Implementation() override;
	virtual void Unselected_Implementation() override;
	virtual void SetTargetPoint_Implementation(const FCommandTarget & Target) override {};
	virtual bool CanBeMultipleSelected_Implementation() override { return false; }
	virtual FControlData GetUIData_Implementation() override;

protected:
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Building")
	int Level;

	UPROPERTY(EditDefaultsOnly, Category = "Building")
	int MaxLevel;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Building")
	TArray<FBuildingLevelData> BuildingData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "TeamObject")
	int TeamNum;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Building")
	float Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building")
	USkeletalMeshComponent* BuildingMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	UBoxComponent* BoundingBoxComponent;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Building")
	TArray<FBuildingWorker> Workers;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BuildProgress, Category = "Building")
	float BuildProgress; // 0.0f - 1.0f

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building")
	TArray<USkeletalMesh*> ProgressBuildMeshes;

private:

	USkeletalMesh* MainBuildingMesh;

	bool CheckWorkerNecessity(class ASimpleWorkerUnit*) const;

	UFUNCTION()
	void OnRep_BuildProgress();
};
