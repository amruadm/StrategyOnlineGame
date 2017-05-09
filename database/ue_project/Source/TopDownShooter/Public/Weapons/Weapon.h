// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS(Blueprintable)
class TOPDOWNSHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Bind(class AGameUnit* owner);

	virtual void Attack(FVector TargetPoint) {};

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float Distance;

	AGameUnit* Owner;

	
protected:	

	UPROPERTY(Category = "Weapon", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;
};
