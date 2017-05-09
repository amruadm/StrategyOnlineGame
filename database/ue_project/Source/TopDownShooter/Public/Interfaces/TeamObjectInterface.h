// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "TopDownShooter.h"
#include "TeamObjectInterface.generated.h"

/**
 * 
 */
UINTERFACE(Blueprintable)
class UTeamObjectInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class ITeamObjectInterface
{
	GENERATED_IINTERFACE_BODY()
public:

	/*
		@return object team identifier
	*/
	virtual int GetTeamNum() const = 0;

	virtual void SetTeamNum(int number) = 0;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "TeamObject")
	void OnMouseIn();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "TeamObject")
	void OnMouseOut();

};