// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Resource/GameResource.h"
#include "Public/Resource/ResourceItem.h"
#include "Public/Units/WorkerUnit.h"


// Sets default values
AGameResource::AGameResource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	
}

// Called when the game starts or when spawned
void AGameResource::BeginPlay()
{
	Super::BeginPlay();
	Count = FMath::RandRange(MinGenerateCount, MaxGenerateCount);
}

// Called every frame
void AGameResource::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AGameResource::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGameResource, Count);
}

float AGameResource::GetExtractionTime_Implementation(AWorkerUnit* Unit) const
{
	return 1.0f;
}

bool AGameResource::CanExtract(AWorkerUnit* Unit) const
{
	return true;
}

int AGameResource::GetExtractionCount_Implementation(AWorkerUnit* Unit) const
{
	return 1;
}

int AGameResource::GetExtractionExp_Implementation(AWorkerUnit* Unit) const
{
	return 1;
}

FItemCeil AGameResource::Extract(AWorkerUnit* Unit)
{
	FItemCeil ResultItem = FItemCeil::EmptyItem;
	if (Count > 0)
	{
		ResultItem.ItemClass = ItemClass;
		int Avaliable = FMath::Min(GetExtractionCount(Unit), Count);
		ResultItem.Count = Avaliable;
		Count -= Avaliable;
		if (Count < 0)
			DieResource();
	}
	return ResultItem;
}

void AGameResource::DieResource()
{
	Destroy();
}