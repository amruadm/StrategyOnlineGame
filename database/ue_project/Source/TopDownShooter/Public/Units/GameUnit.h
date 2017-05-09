// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Public/Interfaces/TeamObjectInterface.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Public/EditorSupportDelegates.h"
#include "GameUnit.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProcessDelegate, EGameUnitAction, ActionType);

UCLASS(Blueprintable, abstract)
class TOPDOWNSHOOTER_API AGameUnit : public ACharacter, public ITeamObjectInterface
{
	GENERATED_BODY()

public:
	/** Current health of this Pawn */
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Unit)
	float Health;

	/** Current Expirience of this Pawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Unit)
	int Exp;

	/** Current Strength of this Pawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Unit)
	int Strength;

	/** Current  Agility of this Pawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Unit)
	int Agility;

	/** Current Intellect of this Pawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Unit)
	int Intellect;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	bool WaitForProcessAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Unit | Animations")
	TMap<EGameUnitAction, FAnimationArray> Anims;

	// Sets default values for this character's properties
	AGameUnit();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
	float GetMaxSpeed() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
	bool HasDied() const;

	UFUNCTION(BlueprintCallable, Category = "GameUnit")
	void BeginProcessAction(EGameUnitAction ActionType, AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "GameUnit")
	virtual void ProcessAction(EGameUnitAction ActionType);

	UFUNCTION(BlueprintCallable, Category = "GameUnit")
	virtual void EndAction(EGameUnitAction ActionType);

	UFUNCTION(BlueprintCallable, Category = "GameUnit")
	void PlaceTo(AActor* NewPlace);

	UFUNCTION(BlueprintCallable, Category = "GameUnit")
	void ExitFromPlace();

	UFUNCTION(BlueprintCallable, Category = "GameUnit")
	bool IsPlaced() const;

	UFUNCTION(BlueprintCallable, Category = "GameUnit")
	AActor* GetPlace() const { return Place; }

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FProcessDelegate OnBeginProcessAction;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FProcessDelegate OnProcessAction;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FProcessDelegate OnEndAction;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Events")
	bool isProcessing;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	virtual void Die(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser);

	virtual int GetTeamNum() const override { return TeamNum; }

	virtual void SetTeamNum(int number) override { TeamNum = number; }

	UFUNCTION()
	void PlayAnimation(EGameUnitAction AnimActionType, int AnimIndex);

	UFUNCTION()
	void PlayRandomAnimation(EGameUnitAction AnimActionType);

private:

	UFUNCTION(NetMulticast, reliable)
	void MulticastPlayAnimation(EGameUnitAction AnimActionType, int AnimIndex);

	void ProcessAttack();
	
protected:

	UFUNCTION(NetMulticast, reliable)
	void MulticastBeginProcessAction(EGameUnitAction ActionType);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category="TeamObject")
	int TeamNum;

	UPROPERTY(Replicated)
	AActor* Target;

	//Building, wagon or airship
	UPROPERTY(Replicated)
	class AActor* Place;
};
