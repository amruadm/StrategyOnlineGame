// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "GamePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class AGamePlayerState : public APlayerState
{
	GENERATED_BODY()
			
public:
	AGamePlayerState();

	uint32 GetTeamNum() const { return TeamNum; }
	void SetTeamNum(uint32 num) { TeamNum = num; }
	
protected:
	UPROPERTY(Replicated)
	uint32 TeamNum;
	
	
};
