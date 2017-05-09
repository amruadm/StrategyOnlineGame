// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Public/Units/WarrirorUnit.h"
#include "TopDownShooterCharacter.generated.h"

UCLASS(Blueprintable)
class ATopDownShooterCharacter : public AWarrirorUnit
{
	GENERATED_BODY()

public:
	ATopDownShooterCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	void MoveToDirection(FVector dir);

	class AWeapon* GetCurrentWeapon() const;
		
	virtual bool ReplicateSubobjects(class UActorChannel * Channel, class FOutBunch * Bunch, FReplicationFlags * RepFlags) override;

	void CreateInventory();

	UPROPERTY(EditDefaultsOnly, Category = ShooterCharacter)
	FName WeaponSocketName;

	UPROPERTY(EditDefaultsOnly, Category = ShooterCharacter)
	TSubclassOf<class AWeapon> MainWeaponClass;
	
	// Inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Character")
	TArray<class UItem*> Inventory;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Character")
	int InventorySize;
				
	//Character weapon and armor items
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Character")
	class UWeaponItem* LeftHandWeaponItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Character")
	class UWeaponItem* RightHandWeaponItem;

protected:
		
	FVector InputTarget;

	UFUNCTION(NetMulticast, Unreliable)
	void ClientSimulateAttack(FVector AttackPoint);

	UFUNCTION(Server, Reliable, WithValidation)
	void Attack(FVector AttackPoint);
	
	void AttachWeapon(AWeapon* weapon);

	bool CanInputMoving() const;

	UPROPERTY(Replicated)
	AWeapon* CurrentWeapon;	

};

