// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BuildingSampler.generated.h"

UCLASS(Blueprintable)
class TOPDOWNSHOOTER_API ABuildingSampler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingSampler();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void Destroyed() override;

	void SetMesh(USkeletalMesh* SamplerMesh);

	void SetColor(FColor Color);

	void SetMeshScale(FVector Scale);

	TSubclassOf<class ABuilding> BuildingClass;

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Building Sampler"), Category = "Building")
	static ABuildingSampler* GetInstance(UWorld* World);

	static void ClearInstance();

private:

	USkeletalMeshComponent* MeshComponent;
	UMaterialInstance* SamplerMaterial;
	
	//Instance of singleton
	static ABuildingSampler* Instance;	
};
