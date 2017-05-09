// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/Weapons/Projecticle.h"
#include "Public/Weapons/ProjecticleWeapon.h"
#include "TopDownShooterCharacter.h"


// Sets default values
AProjecticle::AProjecticle()
{
	//bReplicates = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("SphereTrigger"));
	SphereTrigger->AttachTo(RootComponent);
	SphereTrigger->bGenerateOverlapEvents = true;

	SphereTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereTrigger->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	SphereTrigger->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	SphereTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	ProjecticleParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Projecticle Particles"));
	ProjecticleParticles->AttachTo(SphereTrigger);
}

// Called when the game starts or when spawned
void AProjecticle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjecticle::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	SetActorLocation(GetActorLocation() + FlyingSpeed*DeltaTime*DirectionVector);
}

void AProjecticle::NotifyActorBeginOverlap(AActor* Other)
{
	if (!OwnerWeapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString("!OwnerWeapon"));
		Destroy();
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString("Good notify"));
	ATopDownShooterCharacter* unit = Cast<ATopDownShooterCharacter>(Other);
	if (unit && unit != OwnerWeapon->Owner)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString("HIT!"));
		unit->TakeDamage(OwnerWeapon->Damage, 
						FDamageEvent(), 
						OwnerWeapon->Owner->Controller,	
						OwnerWeapon->Owner);
		if (ExplisionParticles) 
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplisionParticles, GetActorLocation());
		Destroy();
	}
}

void AProjecticle::NotifyActorEndOverlap(AActor* Other)
{

}

void AProjecticle::SetupProjecticle(FVector direction, AProjecticleWeapon* ownerWeapon)
{
	OwnerWeapon = ownerWeapon;
	DirectionVector = direction;
}