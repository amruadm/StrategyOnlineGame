// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Public/GameTypes.h"
#include "Classes/AIController.h"
#include "UnitAIController.generated.h"

class UCrowdFollowingComponent;

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API AUnitAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	AUnitAIController(const FObjectInitializer& ObjectInitializer);

	void SetTarget(const struct FCommandTarget & Target);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AI")
	FCommandTarget GetTarget() const
	{
		return CommandTarget;
	}

protected:

	FORCEINLINE UCrowdFollowingComponent* GetCrowdComponent() const;

	FORCEINLINE AGameUnit* GetControlledUnit() const
	{
		return Cast<AGameUnit>(GetControlledPawn());
	}

	FCommandTarget CommandTarget;
	
};
