// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Buildings/BuildingSampler.h"
#include "Public/Buildings/Building.h"

ABuildingSampler* ABuildingSampler::Instance = nullptr;
// Sets default values
ABuildingSampler::ABuildingSampler()
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Materials/Common/SamplerMaterial.SamplerMaterial'"));
	if (Material.Object)
	{
		MeshComponent->SetMaterial(0, Material.Object);
	}
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABuildingSampler::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ABuildingSampler::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );


}

void ABuildingSampler::SetMesh(USkeletalMesh* SamplerMesh)
{
	MeshComponent->SetSkeletalMesh(SamplerMesh, true);
	//MeshComponent->SkeletalMesh = SamplerMesh;
}

void ABuildingSampler::SetMeshScale(FVector Scale)
{
	//SetActorScale3D(Scale);
	MeshComponent->SetRelativeScale3D(Scale);
	//MeshComponent->SetWorldScale3D(Scale);
}

void ABuildingSampler::SetColor(FColor Color)
{
	MeshComponent->SetVectorParameterValueOnMaterials("Color", (FVector)Color);
}

void ABuildingSampler::Destroyed()
{
	Instance = nullptr;
}

ABuildingSampler* ABuildingSampler::GetInstance(UWorld* World)
{
	if (Instance == nullptr)
	{
		if (World)
		{			
			Instance = World->SpawnActor<ABuildingSampler>(StaticClass());
		}
	}
	return Instance;
}

void ABuildingSampler::ClearInstance()
{
	Instance = nullptr;
}