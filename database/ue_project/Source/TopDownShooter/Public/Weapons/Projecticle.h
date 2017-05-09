// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projecticle.generated.h"

UCLASS(Blueprintable)
class TOPDOWNSHOOTER_API AProjecticle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjecticle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	virtual void NotifyActorBeginOverlap(AActor* Other) override;

	virtual void NotifyActorEndOverlap(AActor* Other) override;

	void SetupProjecticle(FVector direction, class AProjecticleWeapon* ownerWeapon);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projecticle")
	UParticleSystem* ExplisionParticles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projecticle")
	float FlyingSpeed;

	UPROPERTY(Category = "Projecticle", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereTrigger;

	UPROPERTY(Category = "Projecticle", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ProjecticleParticles;

protected:

	FVector DirectionVector;

	AProjecticleWeapon* OwnerWeapon;	
};
