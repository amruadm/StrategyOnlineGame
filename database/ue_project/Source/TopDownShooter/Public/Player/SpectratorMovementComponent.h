// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpectatorPawnMovement.h"
#include "SpectratorMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API USpectratorMovementComponent : public USpectatorPawnMovement
{
	GENERATED_BODY()
	
	USpectratorMovementComponent();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	bool bInitialLocationSet;
	
	
};
