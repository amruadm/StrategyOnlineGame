// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS(Blueprintable)
class TOPDOWNSHOOTER_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	virtual bool CanBePickup(class ATopDownShooterCharacter* pawn) const;

	virtual void NotifyActorBeginOverlap(AActor* Other) override;

	virtual void NotifyActorEndOverlap(AActor* Other) override;

private:

	UPROPERTY()
	UPrimitiveComponent* Trigger;
	
};
