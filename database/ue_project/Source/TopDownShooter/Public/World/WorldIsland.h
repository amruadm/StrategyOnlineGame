// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "WorldIsland.generated.h"

UCLASS()
class TOPDOWNSHOOTER_API AWorldIsland : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AWorldIsland();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Generation")
	int Resolution;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Generation")
	float SegmentWidth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Generation")
	FName DynamicTextureParam;

protected:

	void RegenerateIsland();

	void GenerateMap();

	void GenerateMesh();

	void GenerateBottomMesh();

	bool CheckNeightbours(int x, int y);

	void ResetColors();

	void UpdateMap();

	UFUNCTION()
	void OnRep_RandomSeed();

	UPROPERTY(EditAnywhere, Replicated, ReplicatedUsing = OnRep_RandomSeed, BlueprintReadOnly, Category = "Generation")
	FRandomStream RandStream;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	UProceduralMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, Category = "Mesh")
	UTexture2D* MapTexture;

	FUpdateTextureRegion2D* mUpdateTextureRegion;
	uint8* mDynamicColors;
	float* mDynamicColorsFloat;
	uint32 mDataSize;
	uint32 mDataSqrtSize;
	int32 mRowSize;
	int32 mArraySize;

	const float heightScale = 10.0f;

	UMaterialInstanceDynamic* DynamicMaterial;
};
