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
	Count = FMath::RandRange(MinGenerateCount, MaxGenerateCount);
}

// Called when the game starts or when spawned
void AGameResource::BeginPlay()
{
	Super::BeginPlay();
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

float AGameResource::GetExtractionTime(AWorkerUnit* Unit) const
{
	return 1.0f;
}

bool AGameResource::CanExtract(AWorkerUnit* Unit) const
{
	return true;
}

int AGameResource::GetExtractionCount(AWorkerUnit* Unit) const
{
	return 1;
}

void AGameResource::BeginExtract(AWorkerUnit* Unit)
{
	if (Unit && CanExtract(Unit))
	{
		Unit->BeginProcessAction(ProcessAnimType, this);
		FTimerHandle coolDownTimer;
		FTimerDelegate SpawnDelegate = FTimerDelegate::CreateLambda([=]()
		{
			ProcessExtract(Unit);
		});
		GetWorld()->GetTimerManager().SetTimer(coolDownTimer, SpawnDelegate, GetExtractionTime(Unit), false);
	}
}

void AGameResource::ProcessExtract(AWorkerUnit* Unit)
{
	if (Unit)
	{

		int cnt = GetExtractionCount(Unit);
		FItemCeil item(Count, ItemClass);
		Unit->GiveResourceFrom(item, cnt);
		Count = FMath::Max(0, Count - cnt);
		Unit->ProcessAction(ProcessAnimType);
		if (Count <= 0)
		{
			DieResource();
		}
	}
}

void AGameResource::DieResource()
{
	Destroy();
}